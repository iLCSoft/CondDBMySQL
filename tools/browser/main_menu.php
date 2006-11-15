<?php /*init settings*/
session_start();
require_once("defines.php"); /* default #includes */
define("PHP_LM_PATH", THIRD_PARTY_LIB_PATH . "phplayermenu/"); /* menu package path */
define("PHP_LM_WWW_PATH", THIRD_PARTY_WWW_LIB_PATH . "phplayermenu/"); /* http path */
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html lang="en">

<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Content-Language" content="en">
<meta http-equiv="Content-Style-Type" content="text/css">
<meta http-equiv="Content-Script-Type" content="text/javascript">
<link rel="stylesheet" type="text/css" href="<?php print PHP_LM_WWW_PATH ?>layerstreemenu.css">
<style type="text/css">
<!--
@import url("<?php print PHP_LM_WWW_PATH /*trick for supporting Netscape 4*/ ?>layerstreemenu-hidden.css");
//-->
</style>
<link rel="stylesheet" type="text/css" href="<?php print CSS_WWW_PATH ?>main.css">

<!-- JS Includes //-->
<?php include (PHP_LM_PATH . "libjs/layersmenu-browser_detection.js"); /*trick for supporting Netscape 4*/?>
<?php /*open/close folders script*/ ?>
<script language="JavaScript" type="text/javascript" src="<?php print PHP_LM_WWW_PATH ?>libjs/layerstreemenu-cookies.js"></script>

</head>

<body>
<!--Form used to reload browser-->
<form name="homeForm" method="post" action="index.php" target="_parent">
<input type="hidden" name="restart" value="true" />
</form>

<table align="left" >
<tr><td>
<img src="images/browserLogo.gif" width="130" height="172" border="0" />
</td></tr>
<tr><td>
<div class="menu">
<a href="javaScript:document.forms.homeForm.submit();">Version: <?php echo VERSION?></a>
</div>
</td></tr>
<tr><td>&nbsp</td></tr>
<tr><td>
<?php

require(CLASS_PATH . "condDB.php");

/*check if a connection is changed in the menu and connect to the DB*/
if ($_GET['newConnection']) {
  session_unset();
  if (!empty($_POST['host']) && 
      !empty($_POST['database']) && 
      !empty($_POST['username'])) {
	$_SESSION['host']= $_POST['host'];
	$_SESSION['database']= $_POST['database'];
	$_SESSION['username']= $_POST['username'];
	if (empty($_POST['password'])) {
	  $_SESSION['password']= "";
	}
	else {
	$_SESSION['password']= $_POST['password'];
	}
	$_SESSION['changedDB']= true; //flag indicating the connection is not the default
      }
  $condDB= new condDB($_SESSION['host'], $_SESSION['database'], $_SESSION['username'], $_SESSION['password']);
}
else {
  $condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS); //connect using default
}
$connected= $condDB->connect();

if ($connected) {
  echo "<div class=\"menu\">";

  /*create treemenu*/
  require(PHP_LM_PATH . "lib/PHPLIB.php");
  require(PHP_LM_PATH . "lib/layersmenu-common.inc.php");
  require(PHP_LM_PATH . "lib/treemenu.inc.php");

  $menu = new TreeMenu(); /* call the constructor */
  $menu->setDirroot(PHP_LM_PATH);
  $menu->setLibdir(PHP_LM_PATH . "lib/");
  $menu->setLibjsdir(PHP_LM_PATH . "libjs/");
  $menu->setLibjswww(PHP_LM_WWW_PATH . "libjs/");
  $menu->setImgdir(PHP_LM_PATH . "images/");
  $menu->setImgwww(PHP_LM_WWW_PATH . "images/");
  $tree_string= $condDB->buildTreeMenu();
  if (empty($tree_string)) {
     echo("No records found in the Database");
  }
  else {
       $menu->setMenuStructureString($tree_string);  /* get the menu configuration*/
       $menu->parseStructureForMenu("treemenu1");
       print $menu->newTreeMenu("treemenu1");
  }
  echo "</div>";
}
?>
</td></tr>
<tr><td>&nbsp</td></tr>
<tr><td>
<div class="db-info">
<?php
/*print information about the database*/
if ($_SESSION['changedDB'] && $connected) {
     echo("<b>Host: </b><i>" . $_SESSION['host'] . "</i><br />");
     echo("<b>Database</b><i>: " . $_SESSION['database'] . "</i><br />");
     echo("<b>User: </b><i>" . $_SESSION['username'] ."</i>");
}
elseif (! $connected) {
  echo("Unable to connect to the database!");
}
else {
  echo("<b>Host: </b><i>" . DB_HOST . "</i><br />");
  echo("<b>Database: </b><i>" . DB_NAME. "</i><br />");
  echo("<b>User: </b><i>" . DB_USER . "</i>");
}

?>
</div>
</td></tr>
<tr><td>&nbsp</td></tr>
<tr><td>
<div class="menu">
<table><tr><td>
<?php
require_once(THIRD_PARTY_LIB_PATH . "forms/Forms.php"); /* menu package path */
echo "<tr><td>";
echo Forms::openForm("main_menu.php?newConnection=true");
echo "Host: ";
echo Forms::textField('', "host");
echo "</td></tr><tr><td>";
echo "DB: ";
echo Forms::textField('', "database");
echo "</td></tr><tr><td>";
echo "User: ";
echo Forms::textField('', "username");
echo "</td></tr><tr><td>";
echo "Pass: ";
echo Forms::textField('', 'password', true);
echo "</td></tr>";
echo "<tr><td>&nbsp</td></tr>";
echo "<tr><td>";
echo Forms::submitButton("Change DB");
echo Forms::closeForm();
echo "</tr></td>";
?>
</td></tr></table>
</div>

</tr></td>
</table>

<?php 
//close connection
$condDB->destroy();
?>

</body>
</html>
