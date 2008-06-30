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

#ifndef __OWL_AUDIO_PLAYER_H__
#define __OWL_AUDIO_PLAYER_H__

#include <glib-object.h>
#include <gst/gsttaglist.h>

G_BEGIN_DECLS

#define OWL_TYPE_AUDIO_PLAYER \
                (owl_audio_player_get_type ())
#define OWL_AUDIO_PLAYER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OWL_TYPE_AUDIO_PLAYER, \
                 OwlAudioPlayer))
#define OWL_AUDIO_PLAYER_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_CAST ((klass), \
                 OWL_TYPE_AUDIO_PLAYER, \
                 OwlAudioPlayerClass))
#define OWL_IS_AUDIO_PLAYER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OWL_TYPE_AUDIO_PLAYER))
#define OWL_IS_AUDIO_PLAYER_CLASS(klass) \
                (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                 OWL_TYPE_AUDIO_PLAYER))
#define OWL_AUDIO_PLAYER_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 OWL_TYPE_AUDIO_PLAYER, \
                 OwlAudioPlayerClass))

typedef struct _OwlAudioPlayerPrivate OwlAudioPlayerPrivate;

typedef struct {
        GObject parent;

        OwlAudioPlayerPrivate *priv;
} OwlAudioPlayer;

typedef struct {
        GObjectClass parent_class;

        /* Signals */
        void (* tag_list_available) (OwlAudioPlayer *audio_player,
                                     GstTagList     *tag_list);
        void (* eos)                (OwlAudioPlayer *audio_player);
        void (* error)              (OwlAudioPlayer *audio_player,
                                     GError         *error);
        
        /* Future padding */
        void (* _reserved1) (void);
        void (* _reserved2) (void);
        void (* _reserved3) (void);
        void (* _reserved4) (void);
} OwlAudioPlayerClass;

GType
owl_audio_player_get_type           (void) G_GNUC_CONST;

OwlAudioPlayer *
owl_audio_player_new                (void);

void
owl_audio_player_set_uri            (OwlAudioPlayer *audio_player,
                                     const char     *uri);

const char *
owl_audio_player_get_uri            (OwlAudioPlayer *audio_player);

void
owl_audio_player_set_playing        (OwlAudioPlayer *audio_player,
                                     gboolean        playing);

gboolean
owl_audio_player_get_playing        (OwlAudioPlayer *audio_player);

void
owl_audio_player_set_position       (OwlAudioPlayer *audio_player,
                                     int             position);

int
owl_audio_player_get_position       (OwlAudioPlayer *audio_player);

void
owl_audio_player_set_volume         (OwlAudioPlayer *audio_player,
                                     double          volume);

double
owl_audio_player_get_volume         (OwlAudioPlayer *audio_player);

gboolean
owl_audio_player_get_can_seek       (OwlAudioPlayer *audio_player);

int
owl_audio_player_get_buffer_percent (OwlAudioPlayer *audio_player);

int
owl_audio_player_get_duration       (OwlAudioPlayer *audio_player);

G_END_DECLS

#endif /* __OWL_AUDIO_PLAYER_H__ */
