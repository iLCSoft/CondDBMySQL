<?php

/*
*  This file contains the code related to the browse menu
* that is available on the mainFrame after a folder is
* selected from the main_menu.
*
* Please make comments to tiago@cern.ch
* Author: Gama Franco
*/

require_once("defines.php"); /* default #includes */
require_once(THIRD_PARTY_LIB_PATH . "forms/Forms.php"); /* menu package path */

/*these functions are used to fill the 'selects' of the menu*/
/*Year field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_year_value($name, $current_time= false) {

	 /*check if no value was set and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="yyyy") {
    if ($current_time) {
      echo("value=\"" . gmdate("Y") ."\"");
    }
    else
      echo("value=\"yyyy\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}

/*Month field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_month_value($name, $current_time= false) {
  /*check if no value was set, and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="mm") {
    if ($current_time) {
      echo("value=\"" . gmdate("m") ."\"");
    }
    else
      echo("value=\"mm\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}

/*Day field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_day_value($name, $current_time= false) {
  /*check if no value was set, and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="dd") {
    if ($current_time) {
      echo("value=\"" . gmdate("d") ."\"");
    }
    else
      echo("value=\"dd\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}

/*Hour field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_hour_value($name, $current_time= false) {
 	 /*check if no value was set, and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="hh") {
    if ($current_time) {
      echo("value=\"" . gmdate("H") ."\"");
    }
    else
      echo("value=\"hh\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}

/*Minute field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_minute_value($name, $current_time= false) {
 	 /*check if no value was set, and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="mm") {
    if ($current_time) {
      echo("value=\"" . gmdate("i") ."\"");
    }
    else
      echo("value=\"mm\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}

/*Second field*/
/*@param $name: is the name of the <text> on the form*/
/*@param $current_time: default time is current time?*/
function post_second_value($name, $current_time= false) {
 	 /*check if no value was set, and aply default*/
  if (!isset($_SESSION[$name]) || $_SESSION[$name]=="ss") {
    if ($current_time) {
      echo("value=\"" . gmdate("s") ."\"");
    }
    else
      echo("value=\"ss\"");
  }
  else
    echo("value=\"" . $_SESSION[$name] . "\"");
}


/*
 * returns string in html format for the display
 * of the "Browse last minutes" box
*/
function bMinutesBox() 
{
  $values= array();
  for ($i=0, $j=5; $j<=60; $i++, $j+=5)
    $values[$i]=$j;
  $aditional= "onChange=\"bMinutesTrigger()\" onFocus=\"bMinutesTrigger()\"";
  return Forms::selectBox($values, 0, 'bminutes', 1, false, $aditional);
}
?>

<!-- JS scripts used in this menu //-->

<script language="JavaScript" type="text/javascript" 
		src=<?php echo("\"". JS_LIB_WWW_PATH . "browser_menu.js\"");?> ></script>

<script language="JavaScript" type="text/javascript" 
		src=<?php echo("\"". JS_LIB_WWW_PATH . "date.js\"");?> ></script>

<div class="menu">

<!-- Visual Settings //-->
<table><tr><td>
<form name="visualSettings" method="post" action="main_frame.php?path=<?php echo($_[path])?>" onsubmit="return validateVisualSettings()">
</td></tr>
<tr>
	<td><!-- Browse with tag //-->
		<input type="radio" value="<?php echo(B_TAG)?>" name="visualSetting" onclick="refreshFormState()"
		<?php if ($_SESSION['visualSetting']== B_TAG) echo(" checked "); ?>> Browse with tag
	</td>
	<td><!-- Browse  at point //-->
      <input type="radio" name="visualSetting" value="<?php echo(B_POINT)?>" onclick="refreshFormState()"
		<?php if ($_SESSION['visualSetting']== B_POINT) echo(" checked "); ?> > Browse at point
	</td>
	<td><!-- Browse history //-->
		<input type="radio" name="visualSetting" value="<?php echo(B_HISTORY)?>" onclick="refreshFormState()"
	 	<?php if ($_SESSION['visualSetting']== B_HISTORY) echo(" checked "); ?>> Browse history
	</td>
</tr>
<tr>
	<td><!-- Browse with tag //-->
		Tag:
	</td>	
	<td><!-- Browse  at point //-->
		Point:
	</td>
	<td><!-- Browse history //-->
		Since:
	</td>
</tr>
<tr>
	<td><!-- Browse with tag //-->
		<select name="browseWTag">
		<option <?php if ($_SESSION['tag']=="HEAD" || !isset($_SESSION['tag']) ){ echo "selected=\"selected\"";}?>>HEAD</option> 

		<?php //add reamaining tags
		$tags= $condDB->getTags($_SESSION['folder']);
		foreach ($tags as $tag) {
		  echo("<option " . ($_SESSION['tag']== $tag ? "selected=\"selected\"" : '') . ">" . $tag . "</option> ");
		}
?>
     		</select>
	</td>
	<td><!-- Browse  at point //-->
		<input type="text" size="4" maxlength="4" name="yearBPoint" class="small" 
			onkeyup="nextElement(this, 5)" 
		<?php post_year_value("year", true); ?> />
				
		<input type="text" size="2" maxlength="2" name="monthBPoint" class="small" 
			onkeyup="nextElement(this, 6)" 		
		<?php post_month_value("month", true); ?> />

		<input type="text" size="2" maxlength="2" name="dayBPoint" class="small" 
			onkeyup="nextElement(this, 10)" 
		<?php post_day_value("day", true); ?> />
	</td>
	<td><!-- Browse history //-->

		<input type="text" size="4" maxlength="4" name="yearBHistSince" class="small" 
			onkeyup="nextElement(this, 8)" 
	<?php post_year_value("yearSince", true); ?> />


		<input type="text" size="2" maxlength="2" name="monthBHistSince" class="small" 
			onkeyup="nextElement(this, 9)"
		<?php post_month_value("monthSince", true); ?> />

		<input type="text" size="2" maxlength="2" name="dayBHistSince" class="small" 
			onkeyup="nextElement(this, 13)"
		<?php post_day_value("daySince", true); ?> />
	</td>
</tr>
<tr>
	<td><!-- Browse with tag //-->
	</td>
	<td><!-- Browse  at point //-->
		<input type="text" size="2" maxlength="2" name="hourBPoint" class="small" 
			onkeyup="nextElement(this, 11)"
		<?php post_hour_value("hour", true); ?> />

		<input type="text" size="2" maxlength="2" name="minuteBPoint" class="small" 
			onkeyup="nextElement(this, 12)"
		<?php post_minute_value("minute", true); ?> />

		<input type="text" size="2" maxlength="2" name="secondBPoint" class="small" 
		<?php post_second_value("second", true); ?> />
	</td>
	<td><!-- Browse history //-->
		<input type="text" size="2" maxlength="2" name="hourBHistSince" class="small" 
			onkeyup="nextElement(this, 14)"
		<?php post_hour_value("hourSince", true); ?> />


		<input type="text" size="2" maxlength="2" name="minuteBHistSince" class="small" 
			onkeyup="nextElement(this, 15)"
		<?php post_minute_value("minuteSince", true); ?> />


		<input type="text" size="2" maxlength="2" name="secondBHistSince" class="small" 
			onkeyup="nextElement(this, 16)"
		<?php post_second_value("secondSince", true); ?> />

	</td>
</tr>
<tr>
	<td><!-- Browse with tag //-->
	</td>
	<td><!-- Browse  at point //-->
	</td>
	<td><!-- Browse history //-->
		Till:
	</td>
</tr>
<tr>
	<td><!-- Browse with tag //-->
	</td>
	<td><!-- Browse  at point //-->
	</td>
	<td><!-- Browse history //-->
		<input type="text" size="4" maxlength="4" name="yearBHistTill" class="small" 
			onkeyup="nextElement(this, 17)"
	<?php post_year_value("yearTill", true); ?> />


		<input type="text" size="2" maxlength="2" name="monthBHistTill" class="small" 
			onkeyup="nextElement(this, 18)"
 		<?php post_month_value("monthTill", true);?> />

		<input type="text" size="2" maxlength="2" name="dayBHistTill" class="small" 
			onkeyup="nextElement(this, 19)" 
  		<?php post_day_value("dayTill", true);?> />
	</td>
</tr>	
<tr>
	<td><!-- Browse with tag //-->
	</td>
	<td><!-- Browse  at point //-->
	</td>
	<td><!-- Browse history //-->
		<input type="text" size="2" maxlength="2" name="hourBHistTill" class="small" 
			onkeyup="nextElement(this, 20)"
  		<?php post_hour_value("hourTill", true);?> />

		<input type="text" size="2" maxlength="2" name="minuteBHistTill" class="small" 
			onkeyup="nextElement(this, 21)"
  		<?php post_minute_value("minuteTill", true);?> />

		<input type="text" size="2" maxlength="2" name="secondBHistTill" class="small" 
  		<?php post_second_value("secondTill", true);?> />
	</td>
</tr>	
<tr>
	<td>
		<input type="submit" value="Submit" name="button1" />

		<!-- Hidden value, saving folder type -->
		 <input	type="hidden" value="<?php echo($condDB->getFolderType($_SESSION['folder'])) ?>" name="folderType"/>

	</td>
	<td>
	<input type="checkbox" <?php if($_SESSION['runDisplay']) echo("checked") ?>" name="runDisplay"/> Display Keys as run/event
	</td>
	<td>
	Browse last <?php echo(bMinutesBox());?> minutes
	</td>

</tr>

<tr><td></form></td></tr>
</table>

<?php /*After the form loads refresh DOM state*/?>
<script language="JavaScript" type="text/javascript">
<!--
refreshFormState(<?php echo $_SESSION[visualSetting]?>);
//-->
</script>
</div>
