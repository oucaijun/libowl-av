/*
 * Copyright (C) 2006 OpenedHand Ltd.
 *
 * OpenedHand Widget Library Video Widget - A GStreamer video GTK+ widget
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

#ifndef __OWL_OVERLAY_BIN_H__
#define __OWL_OVERLAY_BIN_H__

#include <gtk/gtkbin.h>

G_BEGIN_DECLS

#define OWL_TYPE_OVERLAY_BIN \
                (owl_overlay_bin_get_type ())
#define OWL_OVERLAY_BIN(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OWL_TYPE_OVERLAY_BIN, \
                 OwlOverlayBin))
#define OWL_OVERLAY_BIN_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_CAST ((klass), \
                 OWL_TYPE_OVERLAY_BIN, \
                 OwlOverlayBinClass))
#define OWL_IS_OVERLAY_BIN(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OWL_TYPE_OVERLAY_BIN))
#define OWL_IS_OVERLAY_BIN_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                 OWL_TYPE_OVERLAY_BIN))
#define OWL_OVERLAY_BIN_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 OWL_TYPE_OVERLAY_BIN, \
                 OwlOverlayBinClass))

typedef struct {
        GtkBin parent;

        /* Future padding */
        gpointer _owl_reserved;
} OwlOverlayBin;

typedef struct {
        GtkBinClass parent_class;

        /* Future padding */
        void (* _owl_reserved1) (void);
        void (* _owl_reserved2) (void);
        void (* _owl_reserved3) (void);
        void (* _owl_reserved4) (void);
} OwlOverlayBinClass;

GType
owl_overlay_bin_get_type (void) G_GNUC_CONST;

GtkWidget *
owl_overlay_bin_new      (void);

G_END_DECLS

#endif /* __OWL_OVERLAY_BIN_H__ */
