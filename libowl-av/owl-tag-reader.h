/*
 * Copyright (C) 2006, 2008 OpenedHand Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 */

#ifndef __OWL_TAG_READER_H__
#define __OWL_TAG_READER_H__

#include <glib-object.h>
#include <gst/gsttaglist.h>

G_BEGIN_DECLS

typedef enum {
        OWL_TAG_READER_ERROR_UNKNOWN_TYPE
} OwlTagReaderError;

GQuark owl_tag_reader_error_quark (void);

#define OWL_TAG_READER_ERROR \
                (owl_tag_reader_error_quark ())

#define OWL_TYPE_TAG_READER \
                (owl_tag_reader_get_type ())
#define OWL_TAG_READER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OWL_TYPE_TAG_READER, \
                 OwlTagReader))
#define OWL_TAG_READER_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_CAST ((klass), \
                 OWL_TYPE_TAG_READER, \
                 OwlTagReaderClass))
#define OWL_IS_TAG_READER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OWL_TYPE_TAG_READER))
#define OWL_IS_TAG_READER_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                 OWL_TYPE_TAG_READER))
#define OWL_TAG_READER_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 OWL_TYPE_TAG_READER, \
                 TOwlagReaderClass))

typedef struct _OwlTagReaderPrivate OwlTagReaderPrivate;

typedef struct {
        GObject parent;

        OwlTagReaderPrivate *priv;
} OwlTagReader;

typedef struct {
        GObjectClass parent_class;

        /* Signals */
        void (* uri_scanned) (OwlTagReader *tag_reader,
                              const char   *uri,
                              GError       *error,
                              GstTagList   *tag_list);

        /* Future padding */
        void (* _reserved1) (void);
        void (* _reserved2) (void);
        void (* _reserved3) (void);
        void (* _reserved4) (void);
} OwlTagReaderClass;

GType
owl_tag_reader_get_type        (void) G_GNUC_CONST;

OwlTagReader *
owl_tag_reader_new             (void);

guint
owl_tag_reader_scan_uri        (OwlTagReader *tag_reader,
                                const char   *uri);

void
owl_tag_reader_cancel_scan_uri (OwlTagReader *tag_reader,
                                guint         scan_id);

G_END_DECLS

#endif /* __OWL_TAG_READER_H__ */
