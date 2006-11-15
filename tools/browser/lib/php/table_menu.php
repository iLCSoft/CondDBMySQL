<?php
/*
*  This file contains the code related to the table menu
* that is available on the mainFrame after a folder with tables
* is selected from the main_menu.
*
* Please make comments to gama.franco@freedevices.org
* Author: Gama Franco
*
*PRE: a condDB must be already created in $condDB
*/

require_once("defines.php"); /* default #includes */
require_once(THIRD_PARTY_LIB_PATH . "forms/Forms.php"); /* menu package path */

// //get table schema
$table= $condDB->getSchema($_SESSION['folder']);

if ($table!=-1 &&
    (in_array(CDBINT, $table->types) ||
    in_array(CDBFLOAT, $table->types) ||
    in_array(CDBDOUBLE, $table->types) ||
    in_array(CDBLONGLONG, $table->types) ||
    in_array(CDBBOOL, $table->types) ||
    in_array(CDBSTRING, $table->types)) ) { //insert the menu, otherwise do nothing
?>

<!-- JS scripts used in this menu //-->

<script language="JavaScript" type="text/javascript" 
		src=<?php echo("\"". JS_LIB_WWW_PATH . "table_menu.js\"");?> ></script>

<?php

echo("<br />");

/*debug*/
// print_r($table);
//echo($_SESSION['folder'] . "\n"); flush();

echo ("<div class=\"menu\">\n");
echo "<table>";
echo("<tr><td>");
$aditional= "name=\"tableMenu\" onsubmit=\"return validateTableMenu()\"";
echo Forms::openForm("main_frame.php", "post", "", false, "", $aditional) . "\n";


/*hiddens that will old the value of the browser_menu.php form*/
echo Forms::hidden("visualSetting") . "\n";
/*run/event display*/
echo Forms::hidden("runDisplay") . "\n";
//browse with tag
echo Forms::hidden("browseWTag") . "\n";
//browse at point
echo Forms::hidden("yearBPoint") . "\n";
echo Forms::hidden("monthBPoint") . "\n";
echo Forms::hidden("dayBPoint") . "\n";
echo Forms::hidden("hourBPoint") . "\n";
echo Forms::hidden("minuteBPoint") . "\n";
echo Forms::hidden("secondBPoint") . "\n";
//browse history
echo Forms::hidden("yearBHistSince"). "\n";
echo Forms::hidden("monthBHistSince") . "\n";
echo Forms::hidden("dayBHistSince") . "\n";
echo Forms::hidden("hourBHistSince") . "\n";
echo Forms::hidden("minuteBHistSince") . "\n";
echo Forms::hidden("secondBHistSince") . "\n";
echo Forms::hidden("yearBHistTill") . "\n";
echo Forms::hidden("monthBHistTill") . "\n";
echo Forms::hidden("dayBHistTill") . "\n";
echo Forms::hidden("hourBHistTill") . "\n";
echo Forms::hidden("minuteBHistTill") . "\n";
echo Forms::hidden("secondBHistTill") . "\n";
//save current folder type
echo Forms::hidden("folderType") . "\n";
//save from an to
echo Forms::hidden("from", 0) . "\n";
 echo Forms::hidden("to",  MAX_INTERVAL) . "\n";
 echo("</td></tr>");
for ($i=0; $i< $table->ncolumns; $i++) {
  $aditional= "onclick= \"switchEnableGroup(" . $i . ")\"";
  echo ("<tr>");

  switch($table->types[$i]) {

    //floats e ints
  case CDBINT:
  case CDBFLOAT:
  case CDBDOUBLE:
  case CDBLONGLONG:
    if (isset($_POST["tableCheckBox" . $i]) ) {//option selected before
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, true, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      $options= array("<", "<=", "=", ">=", ">");
      //get selected option
      switch ($_POST["compareSign" . $i ]) {
      case "<":
	$selected=1;
	break;
      case "<=":
	$selected=2;
	break;
      case "=":
	$selected=3;
	break;
      case ">=":
	$selected=4;
	break;
      case ">":
	$selected=5;
	break;
      }
      echo "<td>" . Forms::selectBox($options, $selected, "compareSign" . $i). "</td>\n";
      echo "<td>" . Forms::textField($_POST["userVal" . $i], "userVal" . $i) . "</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    } 
    else {
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, false, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      $options= array("<", "<=", "=", ">=", ">");
      echo "<td>" . Forms::selectBox($options, 1, "compareSign" . $i, 1, false, "disabled") . "</td>\n";
      echo "<td>" . Forms::textField(0, "userVal" . $i, false, -1, -1, "disabled") . "</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    }
    break;

    //bools
  case CDBBOOL:
    if (isset($_POST["tableCheckBox" . $i]) ) {//options selected before
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, true, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      echo "<td>" . Forms::selectBox(array("="), 1, "compareSign" . $i) . "</td>\n";
      echo "<td>" . Forms::selectBox(array("false", "true"), $_POST["userVal" . $i]== "false" ? 1 : 2
				     , "userVal" . $i) . "</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    }
    else{
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, false, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      echo "<td>" . Forms::selectBox(array("="), 1, "compareSign" . $i, 1, false, "disabled") . "</td>\n";
      echo "<td>" . Forms::selectBox(array("false", "true"), 1, "userVal" . $i, 1, false, "disabled") . "</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    }
    break;


    //strings
  case CDBSTRING:
    if (isset($_POST["tableCheckBox" . $i]) ) {//options selected before
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, true, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      echo "<td>" . Forms::selectBox(array("="), 1, "compareSign" . $i) . "</td>\n";
      echo "<td>" ."\"". Forms::textField($_POST["userVal" . $i], "userVal" . $i) . "\"" . "</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    }
    else{
      echo "<td>" . Forms::checkBox($table->names[$i], "tableCheckBox" . $i, false, $aditional) . "</td>\n";
      echo "<td>" . $table->names[$i] . "</td>";
      echo "<td>" . Forms::selectBox(array("="), 1, "compareSign" . $i, 1, false, "disabled") . "</td>\n";
      echo "<td>" . "\"" . Forms::textField("", "userVal" . $i, false, -1, -1, "disabled") . "\"" ."</td>\n";
      echo "<td>" . Forms::hidden("fieldType" . $i, $table->types[$i]) . "</td>\n";
    }
    break;
  } 
  echo("</tr>\n");
}
$aditional_submit= "name=\"tableMenuSubmit\"";

echo "<tr><td>" . Forms::submitButton("Search", false, "", $aditional_submit) . "</td></tr>\n";

 echo("<td><tr>");
echo Forms::closeForm() . "\n";
 echo("</td></tr>");
echo "</table>";
echo ("</div>");
}

?>