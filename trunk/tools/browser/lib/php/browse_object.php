<?php

//print browser menu
require_once(PHP_LIB_PATH. "browser_menu.php");

echo("<br /> <div class=\"info\">\n");
$object= $condDB->getObject($_GET['path'], $_GET['year'], $_GET['month'], $_GET['day'], $_GET['hour'],
			    $_GET['minute'], $_GET['second'], $_GET['nanosec']);
if ($object==NULL) {
     exit ("Error! Object not found.");
}
else {
  require_once(PHP_LIB_PATH . "cond_utils.php");
  printObject($object);
}
echo("</div>\n");
?>