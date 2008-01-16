/*
 * Copyright (C) 2006 OpenedHand Ltd.
 *
 * OpenedHand Widget Library Video Widget demonstration application
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 */

#include <gtk/gtk.h>
#include <gst/gst.h>

#include "owl-video-widget.h"
#include "owl-overlay-bin.h"
#include "bacon-volume.h"

typedef struct {
        GtkWindow *window;

        OwlVideoWidget *video_widget;

        GtkWidget *toolbar;
        GtkToggleButton *play_pause_button;
        GtkButton *open_file_button;
        BaconVolumeButton *volume_button;
        GtkScale *position_slider;
        GtkLabel *position_label;
        GtkButton *fullscreen_button;

        gboolean changing_position_slider;
        guint position_slider_timeout_id;
        guint unfullscreen_id;
} AppData;

/**
 * WM close button clicked.
 **/
static gboolean
window_delete_event_cb (GtkWindow *window,
                        GdkEvent  *event,
                        gpointer   user_data)
{
        gtk_main_quit ();
        
        return TRUE;
}

/**
 * GstTagList available.
 **/
static void
tag_list_available_cb (OwlVideoWidget *video_widget,
                       GstTagList        *tag_list,
                       AppData           *data)
{
        g_message ("Tag list available");
}

/**
 * End of stream reached.
 **/
static void
eos_cb (OwlVideoWidget *video_widget,
        AppData           *data)
{
        /**
         * Stop playing.
         **/
        gtk_toggle_button_set_active (data->play_pause_button, FALSE);

        /**
         * Reset stream.
         **/
        owl_video_widget_set_position (data->video_widget, 0);
}

/**
 * An error occurred: Show dialog.
 **/
static void
error_cb (OwlVideoWidget *video_widget,
          GError            *error,
          AppData           *data)
{
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new (data->window,
                                         GTK_DIALOG_NO_SEPARATOR,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_CLOSE,
                                         error->message);
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
}

/**
 * Append @time to @str.
 **/
static void
append_time (GString *str,
             int      time)
{
        int hours, minutes, seconds;

        hours = time / 3600;
        seconds = time % 3600;
        minutes = seconds / 60;
        seconds = seconds % 60;

        if (hours > 0) {
                g_string_append_printf (str, "%d:", hours);
                g_string_append_printf (str, "%.2d:", minutes);
        } else
                g_string_append_printf (str, "%d:", minutes);
        g_string_append_printf (str, "%.2d", seconds);
}

/**
 * Update the position label.
 **/
static void
update_position_label (AppData *data,
                       int      position)
{
        int duration;
        GString *str;

        duration = owl_video_widget_get_duration (data->video_widget);

        /**
         * Update position label.
         **/
        str = g_string_new (NULL);

        append_time (str, position);

        g_string_append (str, " / ");

        append_time (str, duration);

        gtk_label_set_text (data->position_label, str->str);

        g_string_free (str, TRUE);
}

/**
 * Duration or position information changed.
 **/
static void
notify_duration_or_position_cb (GObject    *object,
                                GParamSpec *pspec,
                                AppData    *data)
{
        int position, duration;
        GtkAdjustment *adj;

        /**
         * Only run when not dragging the slider.
         **/
        if (data->position_slider_timeout_id)
                return;

        /**
         * Get values.
         **/
        position = owl_video_widget_get_position (data->video_widget);
        duration = owl_video_widget_get_duration (data->video_widget);

        /**
         * Update label.
         **/
        update_position_label (data, position);

        /**
         * Update slider.
         **/
        adj = gtk_range_get_adjustment (GTK_RANGE (data->position_slider));

        adj->upper          = duration;
        adj->step_increment = 1;
        adj->page_increment = 10;

        gtk_adjustment_changed (adj);

        data->changing_position_slider = TRUE;

        gtk_adjustment_set_value (adj, position);

        data->changing_position_slider = FALSE;
}

/**
 * Play/pause button toggled.
 **/
static void
play_pause_button_toggled_cb (GtkToggleButton *button,
                              AppData         *data)
{
        gboolean active;

        active = gtk_toggle_button_get_active (button);

        owl_video_widget_set_playing (data->video_widget,
                                         active);
}

static void
set_has_media (AppData *data, gboolean has_media)
{
        gtk_widget_set_sensitive (GTK_WIDGET (data->play_pause_button),
                                  has_media);
        gtk_widget_set_sensitive (GTK_WIDGET (data->fullscreen_button),
                                  has_media);
        gtk_widget_set_sensitive (GTK_WIDGET (data->position_slider),
                                  has_media);
        gtk_widget_set_sensitive (GTK_WIDGET (data->position_label),
                                  has_media);
}

/**
 * Open File button clicked.
 **/
static void
open_file_button_clicked_cb (GtkButton *button,
                             AppData   *data)
{
        GtkWidget *file_chooser;

        file_chooser =
                gtk_file_chooser_dialog_new ("Choose a File to Play",
                                             data->window,
                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                             GTK_STOCK_CANCEL,
                                             GTK_RESPONSE_REJECT,
                                             GTK_STOCK_OPEN,
                                             GTK_RESPONSE_ACCEPT,
                                             NULL);
        
        switch (gtk_dialog_run (GTK_DIALOG (file_chooser))) {
        case GTK_RESPONSE_ACCEPT:
        {
                char *uri;

                uri = gtk_file_chooser_get_uri
                                        (GTK_FILE_CHOOSER (file_chooser));

                owl_video_widget_set_uri (data->video_widget, uri);

                g_free (uri);

                /**
                 * Start playing.
                 **/
                set_has_media (data, TRUE);
                gtk_toggle_button_set_active (data->play_pause_button, TRUE);

                break;
        }
        default:
                break;
        }

        gtk_widget_destroy (file_chooser);
}

/**
 * Volume button value changed: Update video widget volume.
 **/
static void
volume_button_value_changed_cb (BaconVolumeButton *button,
                                AppData           *data)
{
        owl_video_widget_set_volume (data->video_widget,
                                        bacon_volume_button_get_value (button));
}

static gboolean
window_button_cb (GtkWidget *widget, GdkEventButton *event, AppData *data)
{
        gtk_window_unfullscreen (data->window);
        gtk_widget_show (data->toolbar);

        g_signal_handler_disconnect (widget, data->unfullscreen_id);
        data->unfullscreen_id = 0;

        return TRUE;
}

static void
fullscreen_button_clicked_cb (GtkButton *button,
                              AppData   *data)
{
        gtk_widget_hide (data->toolbar);
        gtk_window_fullscreen (data->window);

        data->unfullscreen_id = g_signal_connect
                (data->window, "button-press-event", G_CALLBACK (window_button_cb), data);
}


/**
 * Seek scheduled: Seek in stream.
 **/
static gboolean
position_slider_timeout (AppData *data)
{
        int value;

        value = (int) gtk_range_get_value (GTK_RANGE (data->position_slider));
        owl_video_widget_set_position (data->video_widget, value);

        data->position_slider_timeout_id = 0;

        return FALSE;
}

#define POSITION_SLIDER_TIMEOUT 100

/**
 * Position slider value changed: Schedule seek.
 **/
static void
position_slider_value_changed_cb (GtkRange *range,
                                  AppData  *data)
{
        if (data->changing_position_slider)
                return;

        if (data->position_slider_timeout_id)
                g_source_remove (data->position_slider_timeout_id);

        data->position_slider_timeout_id =
                g_timeout_add (POSITION_SLIDER_TIMEOUT,
                               (GSourceFunc) position_slider_timeout,
                               data);

        update_position_label (data,
                               gtk_range_get_value (range));
}

/* Nasty hack so that we have a fullscreen button */
static void
ensure_fullscreen_stock (void)
{
#ifndef GTK_STOCK_FULLSCREEN
  /* This must be GTK+ 2.6, so we need to register the fullscreen item
     manually. */
  
#define GTK_STOCK_FULLSCREEN "gtk-fullscreen"

  GtkIconFactory *factory;
  GdkPixbuf *pixbuf;
  GtkIconSet *set;
  
  /* You never know... */
  if (gtk_icon_factory_lookup_default (GTK_STOCK_FULLSCREEN))
    return;
  
  factory = gtk_icon_factory_new ();
  pixbuf = gdk_pixbuf_new_from_file (PKGDATADIR "/gtk-fullscreen.png", NULL);
  if (pixbuf) {
    set = gtk_icon_set_new_from_pixbuf (pixbuf);
    gtk_icon_factory_add (factory, GTK_STOCK_FULLSCREEN, set);
    gtk_icon_factory_add_default (factory);
  }
#endif
}

/**
 * Main.
 **/
int
main (int argc, char **argv)
{
        AppData *data;
        GtkWidget *overlay_bin, *vbox, *hbox, *position_hbox, *alignment,
                  *image, *buttons_hbox;

        /**
         * Init.
         **/
        g_thread_init (NULL);
        gtk_init (&argc, &argv);
        gdk_threads_init ();
        gst_init (&argc, &argv);

        /**
         * AppData structure.
         **/
        data = g_new (AppData, 1);

        /**
         * Create window.
         **/
        data->window = GTK_WINDOW (gtk_window_new (GTK_WINDOW_TOPLEVEL));
        gtk_window_set_title (data->window, "Video Player");
        gtk_window_set_default_size (data->window, 640, 480);
        gtk_widget_add_events (GTK_WIDGET (data->window), GDK_BUTTON_PRESS_MASK);

        g_signal_connect (data->window,
                          "delete-event",
                          G_CALLBACK (window_delete_event_cb),
                          NULL);

        /**
         * Create video widget.
         **/
        data->video_widget = OWL_VIDEO_WIDGET (owl_video_widget_new ());

        g_signal_connect (data->video_widget,
                          "tag-list-available",
                          G_CALLBACK (tag_list_available_cb),
                          data);
        g_signal_connect (data->video_widget,
                          "eos",
                          G_CALLBACK (eos_cb),
                          data);
        g_signal_connect (data->video_widget,
                          "error",
                          G_CALLBACK (error_cb),
                          data);
        g_signal_connect (data->video_widget,
                          "notify::duration",
                          G_CALLBACK (notify_duration_or_position_cb),
                          data);
        g_signal_connect (data->video_widget,
                          "notify::position",
                          G_CALLBACK (notify_duration_or_position_cb),
                          data);

        gtk_container_add (GTK_CONTAINER (data->window),
                           GTK_WIDGET (data->video_widget));

        /**
         * Create VBox inside video widget.
         **/
        vbox = gtk_vbox_new (FALSE, 0);
        gtk_container_set_border_width (GTK_CONTAINER (vbox), 12);
        gtk_container_add (GTK_CONTAINER (data->video_widget), vbox);

        /**
         * Create right alignment in the bottom end of the VBox.
         **/
        alignment = gtk_alignment_new (1.0, 0.0, 0.0, 0.0);
        gtk_box_pack_end (GTK_BOX (vbox), alignment, FALSE, FALSE, 0);

        /**
         * Create HBox inside alignment.
         **/
        hbox = gtk_hbox_new (FALSE, 6);
        gtk_container_add (GTK_CONTAINER (alignment), hbox);

        /**
         * Create button box inside above HBox.
         **/
        buttons_hbox = gtk_hbox_new (TRUE, 6);
        gtk_box_pack_start (GTK_BOX (hbox), buttons_hbox, FALSE, TRUE, 0);

        /**
         * Create Play/Pause button.
         **/
        overlay_bin = owl_overlay_bin_new ();
        gtk_box_pack_start (GTK_BOX (buttons_hbox),
                            overlay_bin, FALSE, TRUE, 0);
        
        data->play_pause_button = GTK_TOGGLE_BUTTON (gtk_toggle_button_new ());
        gtk_container_add (GTK_CONTAINER (overlay_bin),
                           GTK_WIDGET (data->play_pause_button));

        g_signal_connect (data->play_pause_button,
                          "toggled",
                          G_CALLBACK (play_pause_button_toggled_cb),
                          data);

        image = gtk_image_new_from_icon_name ("stock_media-play",
                                              GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (data->play_pause_button), image);

        /**
         * Create Open File button.
         **/
        overlay_bin = owl_overlay_bin_new ();
        gtk_box_pack_start (GTK_BOX (buttons_hbox),
                            overlay_bin, FALSE, TRUE, 0);

        data->open_file_button = GTK_BUTTON (gtk_button_new ());
        gtk_container_add (GTK_CONTAINER (overlay_bin),
                           GTK_WIDGET (data->open_file_button));

        g_signal_connect (data->open_file_button,
                          "clicked",
                          G_CALLBACK (open_file_button_clicked_cb),
                          data);

        image = gtk_image_new_from_stock (GTK_STOCK_OPEN,
                                          GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (data->open_file_button), image);

        /**
         * Create volume button.
         **/
        overlay_bin = owl_overlay_bin_new ();
        gtk_box_pack_start (GTK_BOX (buttons_hbox),
                            overlay_bin, FALSE, TRUE, 0);

        data->volume_button = BACON_VOLUME_BUTTON
                (bacon_volume_button_new (GTK_ICON_SIZE_LARGE_TOOLBAR,
                                          0.0, 4.0, 0.1));
        gtk_container_add (GTK_CONTAINER (overlay_bin),
                           GTK_WIDGET (data->volume_button));

        gtk_button_set_relief (GTK_BUTTON (data->volume_button),
                               GTK_RELIEF_NORMAL);
        bacon_volume_button_set_value
                        (data->volume_button,
                         owl_video_widget_get_volume (data->video_widget));

        g_signal_connect (data->volume_button,
                          "value-changed",
                          G_CALLBACK (volume_button_value_changed_cb),
                          data);

        /**
         * Create slider + position label box.
         **/
        overlay_bin = owl_overlay_bin_new ();
        gtk_box_pack_start (GTK_BOX (hbox), overlay_bin, FALSE, TRUE, 0);

        position_hbox = gtk_hbox_new (FALSE, 6);
        gtk_container_add (GTK_CONTAINER (overlay_bin), position_hbox);

        gtk_container_set_border_width (GTK_CONTAINER (position_hbox), 6);

        /**
         * Create position slider.
         **/
        data->position_slider = GTK_SCALE (gtk_hscale_new (NULL));
        gtk_box_pack_start_defaults (GTK_BOX (position_hbox),
                                     GTK_WIDGET (data->position_slider));

        gtk_widget_set_size_request
                (GTK_WIDGET (data->position_slider), 250, -1);
        gtk_scale_set_draw_value (data->position_slider, FALSE);

        data->changing_position_slider   = FALSE;
        data->position_slider_timeout_id = 0;

        g_signal_connect (data->position_slider,
                          "value-changed",
                          G_CALLBACK (position_slider_value_changed_cb),
                          data);

        /**
         * Create position label.
         **/
        data->position_label = GTK_LABEL (gtk_label_new (""));
        gtk_box_pack_start_defaults (GTK_BOX (position_hbox),
                                     GTK_WIDGET (data->position_label));
        notify_duration_or_position_cb (G_OBJECT (data->video_widget),
                                        NULL, data);

        /**
         * Create fullscreen button.
         **/
        overlay_bin = owl_overlay_bin_new ();
        gtk_box_pack_start (GTK_BOX (buttons_hbox),
                            overlay_bin, FALSE, TRUE, 0);

        data->fullscreen_button = GTK_BUTTON (gtk_button_new ());
        gtk_container_add (GTK_CONTAINER (overlay_bin),
                           GTK_WIDGET (data->fullscreen_button));

        g_signal_connect (data->fullscreen_button,
                          "clicked",
                          G_CALLBACK (fullscreen_button_clicked_cb),
                          data);
        
        ensure_fullscreen_stock ();
        image = gtk_image_new_from_stock (GTK_STOCK_FULLSCREEN,
                                          GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add (GTK_CONTAINER (data->fullscreen_button), image);

        data->toolbar = hbox;

        /**
         * Go!
         **/
        if (argc > 1) {
                owl_video_widget_set_uri (data->video_widget, argv[1]);
                set_has_media (data, TRUE);
                gtk_toggle_button_set_active (data->play_pause_button, TRUE);
        } else {
                set_has_media (data, FALSE);
        }
        
        gtk_widget_show_all (GTK_WIDGET (data->window));

        gtk_main ();

        /**
         * Cleanup.
         **/
        gtk_widget_destroy (GTK_WIDGET (data->window));

        g_free (data);

        return 0;
}
