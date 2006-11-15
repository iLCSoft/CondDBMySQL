<?php

$folder= $condDB->getFolder($_SESSION['folder']);

if ($folder==NULL) {
  echo("<div class=\"info\">\n Error! Folder not found. </div>\n");
}
else {
  //print browser menu
  require_once(PHP_LIB_PATH. "browser_menu.php");

  //check if we have a table folder and design the tableMenu
  if ($folder->type==STRUCT || $folder->type==STRUCTID || $folder->type==STRUCTTAG) {
    require_once(PHP_LIB_PATH. "table_menu.php");
  }

  /*print folder description*/
  echo("<br/>\n <div class=\"info\">\n");
  require_once(PHP_LIB_PATH . "cond_utils.php");
  printFolder($folder, $_GET['folderSet']);
  echo("</div>\n");
}
?>