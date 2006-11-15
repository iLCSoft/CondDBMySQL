<?php

$folder= $condDB->getFolder($_SESSION['folder']);

if ($folder==NULL) {
  echo("<div class=\"info\">\n Error! Folder not found. </div>\n");
}
else {
  echo("<br/>\n <div class=\"info\">\n");
  require_once(PHP_LIB_PATH . "cond_utils.php");
  printFolder($folder, $_GET['folderSet']);
  echo("</div>\n");
}
?>