<?php
/*This file is used to display objects selected with the option browse point.
*/

//print browser menu
require_once(PHP_LIB_PATH. "browser_menu.php");

//print table menu
require_once(PHP_LIB_PATH. "table_menu.php");

flush(); //force display of the menus, looks nice for the user

$table= $condDB->getTableHistory($_SESSION['folder'], $_SESSION['yearSince'],
				 $_SESSION['monthSince'], $_SESSION['daySince'],
				 $_SESSION['hourSince'], $_SESSION['minuteSince'],
				 $_SESSION['secondSince'], $_SESSION['yearTill'], 
				 $_SESSION['monthTill'], $_SESSION['dayTill'],
				 $_SESSION['hourTill'], $_SESSION['minuteTill'],
				 $_SESSION['secondTill'], $_SESSION['from'], 
				 $_SESSION['to'], $_SESSION['cond_table_where_clause']);

//print number of rows found				
echo("<br/>\n <div class=\"info\">\n");
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
?>