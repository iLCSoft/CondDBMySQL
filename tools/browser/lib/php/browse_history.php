<?php
/*This file is used to display objects selected with the option browse point.
*/

//print browser menu
require_once(PHP_LIB_PATH. "browser_menu.php");

$objects= $condDB->getHistory($_SESSION['folder'], $_SESSION['yearSince'],
			      $_SESSION['monthSince'], $_SESSION['daySinte'],
			      $_SESSION['hourSince'], $_SESSION['minuteSince'],
			      $_SESSION['secondSince'], $_SESSION['yearTill'], 
			      $_SESSION['monthTill'], $_SESSION['dayTill'],
			      $_SESSION['hourTill'], $_SESSION['minuteTill'],
			      $_SESSION['secondTill']); /*debug missing from. to*/
												
echo("<br/> <div class=\"info\">\n");
require_once(PHP_LIB_PATH . "cond_utils.php");
printObjects($objects);
echo("\n</div>\n");
?>