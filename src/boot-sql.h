/*
 Copyright (C) 1999-2003 FreeDevices.org  geral@freedevices.org

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id: boot-sql.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "boot-sql.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//

#ifndef _BOOT_SQL_H
#define _BOOT_SQL_H

/*
 *  DBL1
 */

#define FOLDERS_TBL "folders_tbl"
#define FOLDERS_TBL_SCHEMA \
"(\
  fld_id int(11) NOT NULL auto_increment,\
  fparent int(11) NOT NULL default 0,\
  insert_t timestamp,\
  fpath varchar(255) NOT NULL,\
  fdesc varchar(255) NOT NULL,\
  fattr varchar(255) NOT NULL,\
  ddtype int default 0,\
  db_id int(11),\
  is_set bool default 0,\
  PRIMARY KEY (fld_id)\
) ENGINE=MyISAM"

#define DATABASES_TBL "databases_tbl"
#define DATABASES_TBL_SCHEMA \
"(\
  db_id int(11) NOT NULL auto_increment,\
  dbname char(64),\
  srvname char(64),\
  username char(16),\
  password char(16),\
  PRIMARY KEY (db_id)\
) ENGINE=MyISAM"

#define TAGS_TBL "tags_tbl"
#define TAGS_TBL_SCHEMA \
"(\
  tag_id int(11) NOT NULL auto_increment,\
  insert_t timestamp,\
  tname varchar(64) default NULL,\
  tattr varchar(64) default NULL,\
  tdesc varchar(255) default NULL,\
  PRIMARY KEY (tag_id)\
) ENGINE=MyISAM"

#define TAG2FOLDER_TBL "tag2folder_tbl"
#define TAG2FOLDER_TBL_SCHEMA \
"(\
  tag_id int(11) NOT NULL,\
  fld_id int(11) NOT NULL,\
  insert_t timestamp\
) ENGINE=MyISAM"


/*
 *  DBL2
 */

#define OBJECT_KEY_TBL_N "object_key_tbl_"
#define OBJECT_KEY_TBL_SCHEMA \
"(\
  obj_id int(11) NOT NULL auto_increment,\
  insert_t timestamp,\
  since_t bigint,\
  till_t bigint,\
  layer int(11),\
  db_id int(11),\
  dat_id int(11),\
  crypt TINYBLOB,\
  PRIMARY KEY (obj_id),\
  INDEX isin (since_t),\
  INDEX itil (till_t)\
) ENGINE=MyISAM"

#define TABLE_KEY_TBL_SCHEMA \
"(\
  obj_id int(11) NOT NULL auto_increment,\
  insert_t timestamp,\
  since_t bigint,\
  till_t bigint,\
  layer int(11),\
  db_id int(11),\
  dat_id int(11),\
  crypt TINYBLOB,\
  PRIMARY KEY (obj_id),\
  INDEX isin (since_t),\
  INDEX itil (till_t)\
) ENGINE=MyISAM"

#define OBJECT_KEY_TBL_HEAD_N "object_key_tbl_head_"
#define OBJECT_KEY_TBL_HEAD_SCHEMA \
"(\
  obj_id int(11) NOT NULL,\
  since_t bigint,\
  till_t bigint,\
  INDEX iobjh (obj_id),\
  INDEX isinh (since_t),\
  INDEX itilh (till_t)\
) ENGINE=MyISAM"

#define TABLE_KEY_TBL_HEAD_SCHEMA \
"(\
  obj_id int(11) NOT NULL,\
  since_t bigint,\
  till_t bigint,\
  INDEX iobjh (obj_id),\
  INDEX isinh (since_t),\
  INDEX itilh (till_t)\
) ENGINE=MyISAM"

#define TAG2OBJ_TBL_N "tag2obj_tbl_"
#define TAG2OBJ_TBL_SCHEMA \
"(\
  tag_id int(11) NOT NULL,\
  obj_id int(11) NOT NULL,\
  since_t bigint,\
  till_t bigint,\
  INDEX iobjt (obj_id),\
  INDEX isint (since_t),\
  INDEX itilt (till_t)\
) ENGINE=MyISAM"

#define PARTITION_TBL_N "partition_tbl_"
#define PARTITION_TBL_SCHEMA \
"(\
  part_id int(11) NOT NULL auto_increment,\
  since_t bigint,\
  till_t bigint,\
  db_id int(11) NOT NULL default 0,\
  PRIMARY KEY (part_id),\
  KEY (since_t),\
  KEY (till_t)\
) ENGINE=MyISAM"

#define DATA_TBL_ONL_N "data_tbl_onl_"

#define DATA_TBL_TAG_N "data_tbl_tag_"

/*
 *  DBL3
 */

#define DATA_TBL_NN "data_tbl_"
#define DATA_TBL_SCHEMA \
"(\
  dat_id int(11) NOT NULL auto_increment,\
  description varchar(255),\
  oblock longblob,\
  PRIMARY KEY (dat_id)\
) ENGINE=MyISAM"


/*#define LOG_TBL "log_tbl"
#define LOG_TBL_SCHEMA \
"(time timestamp default NULL, \
 description varchar(255)\
) ENGINE=MyISAM"
*/

#endif /* _BOOT_SQL_H */







