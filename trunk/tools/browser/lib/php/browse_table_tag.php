<?php
/*This file is used to display tables selected with the option
	browse tag.
*/

//print browser menu
require_once(PHP_LIB_PATH. "browser_menu.php");

//print table menu
require_once(PHP_LIB_PATH. "table_menu.php");
flush(); //force display of the menus, looks nice for the user


if ($condDB->getFolderType($_SESSION['folder']) != STRUCTTAG) { //table without tags
  echo("<br/>\n <div class=\"info\">\n");
  echo("<p>The given table does not support tags</p>\n");
  echo("\n</div>\n");
}
else {
  $table= $condDB->getTableTag($_SESSION['folder'], $_SESSION['tag'], 
			       $_SESSION['from'], $_SESSION['to'], 
			       $_SESSION['cond_table_where_clause']);

  //print number of rows found				
  echo("<br/>");
  echo("<div class=\"info\">\n");
  echo("Found " . (isset($table->nrows) ? $table->nrows : 0) . " row(s).");
  echo("\n</div><br/>\n");
  
  //get navigations string
  require_once(PHP_LIB_PATH . "utils.php");
  $url="main_frame.php?browser_state=" . S_TABLE_HISTORY;
  $index= print_index_footer($url,$_SESSION['from'], $_SESSION['to'], $table->nrows);
  
  //print navigation
  if (!empty($index)) {
    echo("\n<div class=\"info\">\n");
    echo($index);
    echo("\n</div>");
    echo("\n<br/>");
  }

  /*print table*/
  echo("<div class=\"info\">\n");
  require_once(PHP_LIB_PATH . "cond_utils.php");
  printTable($table);
  echo("\n</div>");
  echo("\n<br/>");

  //print navigation
  if (!empty($index)) {
    echo("\n<div class=\"info\">\n");
    echo($index);
    echo("\n</div>");
    echo("\n<br/>");
  }
}
?>
