/*
 * Copyright (C) 2006 OpenedHand Ltd.
 *
 * OpenedHand Widget Library Video Widget - A GStreamer video GTK+ widget
 * Copyright (C) 2006  OpenedHand Ltd.
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

#include "owl-overlay-bin.h"

G_DEFINE_TYPE (OwlOverlayBin,
               owl_overlay_bin,
               GTK_TYPE_BIN);

static void
owl_overlay_bin_init (OwlOverlayBin *overlay_bin)
{
        /**
         * We do have our own GdkWindow.
         **/
        GTK_WIDGET_UNSET_FLAGS (overlay_bin, GTK_NO_WINDOW);
}

static void
owl_overlay_bin_realize (GtkWidget *widget)
{
        OwlOverlayBin *overlay_bin;
        GdkWindow *parent_window;
        GdkWindowAttr attributes;
        guint attributes_mask;
        int border_width;

        overlay_bin = OWL_OVERLAY_BIN (widget);

        /**
         * Mark widget as realized.
         **/
        GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);

        /**
         * Create our GdkWindow.
         **/
        border_width = GTK_CONTAINER (widget)->border_width;

        attributes.x = widget->allocation.x + border_width;
        attributes.y = widget->allocation.y + border_width;
        attributes.width = widget->allocation.width - border_width * 2;
        attributes.height = widget->allocation.height - border_width * 2;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;
        attributes.visual = gtk_widget_get_visual (widget);
        attributes.colormap = gtk_widget_get_colormap (widget);
        attributes.event_mask = GDK_EXPOSURE_MASK;

        attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

        parent_window = gtk_widget_get_parent_window (widget);
        widget->window = gdk_window_new (parent_window,
                                         &attributes,
                                         attributes_mask);
        gdk_window_set_user_data (widget->window, widget);

        /**
         * Attach GtkStyle.
         **/
        widget->style = gtk_style_attach (widget->style, widget->window);
        gtk_style_set_background (widget->style,
                                  widget->window,
                                  GTK_STATE_NORMAL);
}

static void
owl_overlay_bin_size_request (GtkWidget      *widget,
                                  GtkRequisition *requisition)
{
        int border_width;
        GtkWidget *child;

        border_width = GTK_CONTAINER (widget)->border_width;

        /**
         * Request width from child.
         **/
        child = GTK_BIN (widget)->child;
        if (child && GTK_WIDGET_VISIBLE (child))
                gtk_widget_size_request (child, requisition);

        requisition->width  += border_width * 2;
        requisition->height += border_width * 2;
}

static void
owl_overlay_bin_size_allocate (GtkWidget     *widget,
                                   GtkAllocation *allocation)
{
        OwlOverlayBin *overlay_bin;
        int border_width;
        GtkAllocation child_allocation;
        GtkWidget *child;

        overlay_bin = OWL_OVERLAY_BIN (widget);

        /**
         * Cache the allocation.
         **/
        widget->allocation = *allocation;

        /**
         * Calculate the size for our GdkWindow and for the child.
         **/
        border_width = GTK_CONTAINER (widget)->border_width;

        child_allocation.x      = allocation->x + border_width;
        child_allocation.y      = allocation->y + border_width;
        child_allocation.width  = allocation->width - border_width * 2;
        child_allocation.height = allocation->height - border_width * 2;

        /**
         * Resize our GdkWindow.
         **/
        if (GTK_WIDGET_REALIZED (widget)) {
                gdk_window_move_resize (widget->window,
                                        child_allocation.x,
                                        child_allocation.y,
                                        child_allocation.width,
                                        child_allocation.height);
        }

        /**
         * Forward the size allocation to our child.
         **/
        child = GTK_BIN (widget)->child;
        if (child && GTK_WIDGET_VISIBLE (child)) {
                /**
                 * The child is positioned relative to its parent.
                 **/
                child_allocation.x = 0;
                child_allocation.y = 0;

                gtk_widget_size_allocate (child, &child_allocation);
        }
}

static void
owl_overlay_bin_class_init (OwlOverlayBinClass *klass)
{
        GtkWidgetClass *widget_class;

        widget_class = GTK_WIDGET_CLASS (klass);

        widget_class->realize       = owl_overlay_bin_realize;
        widget_class->size_request  = owl_overlay_bin_size_request;
        widget_class->size_allocate = owl_overlay_bin_size_allocate;
}

/**
 * owl_overlay_bin_new
 *
 * Return value: A new #OwlOverlayBin.
 **/
GtkWidget *
owl_overlay_bin_new (void)
{
        return g_object_new (OWL_TYPE_OVERLAY_BIN, NULL);
}
