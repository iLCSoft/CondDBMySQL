<?php /*init settings*/
/*start session*/
session_start();
require_once("defines.php"); /* default #includes */

//connect to conditions
require_once(CLASS_PATH . "condDB.php");
$condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS);
if (! $condDB->connect()) {
     die("Unable to connect to BD");
}

//detect browser state and load env vars
require_once(PHP_LIB_PATH . "utils.php");
build_env($condDB);

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="autor" content="Gama Franco">
<meta name="description" content="Atlas-DAQ">
<meta name="keywords" content="Atlas, DAQ, CERN">
<link rel="stylesheet" type="text/css" href="<?php print CSS_WWW_PATH?>main.css">
<title>Atlas-DAQ</title>
</head>
<body>
<?php 

/*print working folder*/
if ($_SESSION['browser_state']!=S_START) {
  echo("<div class=\"title\">\n");
  echo("<center><strong>Selected folder: </strong>" . $_SESSION['folder'] . "\n");
  echo("</center></div><br />\n");
}

dispatch_http_request($condDB);


//close connection
$condDB->destroy();

/*debug get object*/
// flush();
// require_once(CLASS_PATH . "condDB.php");
// require("defines.php"); /* default #includes */
// $condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS);
// $object= $condDB->getObject("/cal/temp", 1900, 1, 1, 0, 0, 0, 1);
// require_once(PHP_LIB_PATH . "cond_utils.php");
// printObject($object);
/*debug ends*/


/*debug get tag*/
// flush();
// require_once(CLASS_PATH . "condDB.php");
// require("defines.php"); /* default #includes */
// $condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS);
// //print_r($condDB->getTags("/muon/dcs/tableTag")); /*debug get tags*/
// $condDB->connect();
// $objects= $condDB->getTableTag("/muon/dcs/tableTag", "HEAD");
// // var_dump($objects->rows[0]);
// require_once(PHP_LIB_PATH . "cond_utils.php");
// printTable($objects); //not for blobs
// printObjects($objects); //not for tables
/*debug ends*/

/*debug browse point*/
// flush();
// require_once(CLASS_PATH . "condDB.php");
// require("defines.php"); default #includes
// $condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS);
// $objects= $condDB->getPoint("/muon/dcs/tableTag", 1900, 1, 1, 1, 1, 1);
// require_once(PHP_LIB_PATH . "cond_utils.php");
// printTable($objects); not for blobs
// // printObjects($objects); not for tables
/*debug ends*/

/*debug browse history*/
// flush();
// require_once(CLASS_PATH . "condDB.php");
// require("defines.php"); /* default #includes */
// $condDB= new condDB(DB_HOST, DB_NAME, DB_USER, DB_PASS);
// $condDB->connect();
// $objects= $condDB->getTableHistory("/muon/dcs/MDT_EC_21:eca.", 1900, 1, 1, 0, 0, 1, 2010, 1, 20, 2, 1, 20, 0, 2, "devID = 1");
// require_once(PHP_LIB_PATH . "cond_utils.php");
// printTable($objects); //not for blobs
// printObjects($objects); //not for tables
/*debug ends*/

?>
</script>


</body>

</html>
