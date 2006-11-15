<?php
/* Some functions usefull to operate with data from the conditions database. */
//require_once(ROOT_PATH . "defines.php");

/* Prints an object in HTML format. Object is the output from the Database Bind.
	An array like: [since][till][insertion_time][layer][data][description]
*/

/*prints an array of objects*/
function printObjects($objects)
{
  
  if (empty($objects)) {
    echo("<p> No objects found. </p>");
    return;
  }
  echo("<table>\n");

  /*header row*/
  echo("<tr class=\"header\">\n");
  echo("<td> </td>\n");
  if ($_SESSION['runDisplay']) {//display condDBKeys as run/event
    echo("<td>Since Run:</td>\n");
    echo("<td>Since Event:</td>\n");
    echo("<td>Till Run:</td>\n");
    echo("<td>Till Event:</td>\n");
  }
  else {
    echo("<td>Since (GMT):</td>\n");
    echo("<td>Till (GMT):</td>\n");
  }
  echo("<td>Insertion Time (GMT):</td>\n");
  echo("<td>Layer:</td>\n");
  echo("<td>Description:</td>\n");
  echo("<td>Data:</td>\n");
  echo("</tr>");

  foreach ($objects as $key => $obj)
    printObjects_aux($obj, ++$num);

  /*footer row*/
  echo("<tr class=\"footer\">\n");
  echo("<td> </td>\n");
  if ($_SESSION['runDisplay']) {//display condDBKeys as run/event
    echo("<td>Since Run:</td>\n");
    echo("<td>Since Event:</td>\n");
    echo("<td>Till Run:</td>\n");
    echo("<td>Till Event:</td>\n");
  }
  else {
    echo("<td>Since (GMT):</td>\n");
    echo("<td>Till (GMT):</td>\n");
  }
  echo("<td>Insertion Time:</td>\n");
  echo("<td>Layer:</td>\n");
  echo("<td>Description:</td>\n");
  echo("<td>Data:</td>\n");
  echo("</tr>");

  echo("</table>\n");
}

/*auxiliar function for printBojects*/
function printObjects_aux($object, $num)
{

  if ( $num%2 == 0 ) { //check if it is an even or odd line
    echo("<tr class=\"even\">\n");
  }
  else {
    echo("<tr class=\"odd\">\n");
  }

  if (empty($object->data)) //object empty, no link
      echo("<td>" . $num . "</td>\n");
  else {
    $link= ROOT_WWW_PATH . "main_frame.php" . "?obj_detail=true"; //link to file
    if ($object->since== "-Inf") {
      $link.= "&path=" . $_SESSION['folder'] . "&year=" . 1900 . "&month=" . 1 . "&day=" . 1
	. "&hour=" . 0 . "&minute=" . 0 . "&second=" . 0 . 
	"&nanosec=" . 0 ; //add parameters
    }
    else{
    $link.= "&path=" . $_SESSION['folder'] . "&year=" . $object->year . "&month=" . $object->month . "&day=" . $object->day
      . "&hour=" . $object->hour . "&minute=" . $object->minute . "&second=" . $object->second . 
      "&nanosec=" . $object->nanosec; //add parameters
    }
    echo("<td><a href= \"" . $link . "\">" . $num . "</a></td>\n"); //post data with link to object
  }


  if ($_SESSION['runDisplay']) { //display keys as run/event
    //since run
    echo("<td>" . $object->sinceRun . "</td>\n");
    //since event
    echo("<td>" . $object->sinceEvent . "</td>\n");
    //till run
    echo("<td>" . $object->tillRun . "</td>\n");
    //till event
    echo("<td>" . $object->tillEvent . "</td>\n");
  }
  else { //display condDBKey as time
    //since
    $string= $object->since;
    $string=str_replace("(GMT)", "", $string); //remove GTM tag
    echo("<td>" . $string . "</td>\n");

    //till
    $string= $object->till;
    $string=str_replace("(GMT)", "", $string); //remove GTM tag
    echo("<td>" . $string . "</td>\n");
  }

  //insertion time
  $string= $object->insTime;
  $string=str_replace("(GMT)", "", $string); //remove GTM tag
  echo("<td>" . $string . "</td>\n");
  
  //layer
  echo("<td>" . $object->layer . "</td>\n");

  //description
  echo("<td>" . $object->description . "</td>\n");
  
  //data
  if (empty($object->data)) //empty object?
    echo("<td>Empty Object</td>\n");
  else if (isNova($object->data)) {
    echo("<td><a href= \"" . $link . "\">NOVA reference</a></td>\n"); //post data with link to object
  }
  else
    echo("<td><a href= \"" . $link . "\">" . $object->data . "</a></td>\n"); //post data with link to object

  echo("</tr>\n");	
}

/*prints an object into the document*/
function printObject($object)
{
  echo("<table>\n");
  echo("<tr class=\"header\">\n");
  echo("<td>Location: </td> <td>" . $_SESSION['folder'] . "</td>\n");
  echo("</tr>");

  /*print description*/
  echo("<tr class=\"even\">\n");
  echo("<td>Description: </td> <td>" . $object->description);
  if (isNova($object->data)) /*check if is a nova database*/
    echo(" (Nova DB)");
  echo("</td>\n");
  echo("</tr>");

  echo("<tr class=\"odd\">\n");
  echo("<td>Since:</td> <td>" . $object->since . "</td> \n");
  echo("</tr>");

  echo("<tr class=\"even\">\n");
  echo("<td>Till:</td> <td>" . $object->till . "</td> \n");
  echo("</tr>");

  echo("<tr class=\"odd\">\n");
  echo("<td>Insertion Time:</td> <td>" . $object->insTime . "</td> \n");
  echo("</tr>");

  echo("<tr class=\"even\">\n");
  echo("<td>Layer:</td> <td>" . $object->layer . "</td> \n");
  echo("</tr>");

  /*print data*/
  echo("<tr class=\"odd\">\n");
  echo("<td>Data:</td> <td>");
  if (isNova($object->data)) /*check if we have a Nova Database in payload*/
    printNovaURL($object->data);
  else
    echo($object->data);
  echo("</td> \n");
  echo("</tr>");

  echo("</table>\n");
}

/*prints NOVA object browser link into the document*/
/*PRE: $XMLstring is a NOVA reference*/
function printNovaURL($XMLstring)
{
  //old reference type, bad formed XML
  if (strcasecmp(substr($XMLstring, 0, 10), "<database>")==0){
    $URL=ROOT_WWW_PATH . "main_frame.php?nova=true";
    $parser = xml_parser_create();
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
    $data = "<blob>".$XMLstring."</blob>";
    xml_parse_into_struct($parser, $data, $values, $tags);
    xml_parser_free($parser);
    echo("<a href= \"");
    foreach ($tags as $key=>$val) {
      if ($key == "database") {
	echo "$URL&novaDB=".$values[$val[0]]['value'];
      }
      if ($key == "ref") {
	echo "&ID=".$values[$val[0]]['value'];
      }
    }
    echo("&from=0&to=" . (MAX_INTERVAL-1) . "\">" . htmlspecialchars($XMLstring) . "</a> (Nova DB)");
  }
  else { //well formed xml
    $URL=ROOT_WWW_PATH . "main_frame.php?nova=true";
    $parser = xml_parser_create();
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
    xml_parse_into_struct($parser, $XMLstring, $values, $tags);
    xml_parser_free($parser);
    echo("<a href= \"");
    foreach ($tags as $key=>$val) {
      if ($key == "database") echo "$URL&novaDB=".$values[$val[0]]['value'];
      if ($key == "ref") echo "&ID=".$values[$val[0]]['value'];
    }
    echo("&from=0&to=" . (MAX_INTERVAL-1) . "\">" . htmlspecialchars($XMLstring) . "</a> (Nova DB)");
  }
}


/*check if we have a Nova Database in payload*/
function isNova($payload) {
  if (strcasecmp(substr($payload, 0, 10), "<database>")==0)
    return true;
  else if (strcasecmp(substr($payload, 0, 6), "<nova>")==0)
    return true;
  
  return false;
}

function printRows($table) {

  foreach ($table->rows as $i => $row) {
    if ( $i%2 == 0 ) { //check if it is an even or odd line
      echo("<tr class=\"even\">\n");
    }
    else {
      echo("<tr class=\"odd\">\n");
    }
    echo("<td>[" . ($row->row_number) . "]</td>\n");

    if ($_SESSION['runDisplay']) { //display condDBKey 
      //since run
      echo("<td>" . $row->sinceRun . "</td>\n");
      //since event
      echo("<td>" . $object->sinceEvent . "</td>\n");
      //till run
      echo("<td>" . $object->sinceRun . "</td>\n");
      //till event
      echo("<td>" . $object->sinceEvent . "</td>\n");      
    }
    else {       //print condDBKey as times
      //since
      $string= $row->since;
      $string=str_replace("(GMT)", "", $string); //remove GTM tag
      echo("<td>" . $string . "</td>\n");
      //till
      $string= $row->till;
      $string=str_replace("(GMT)", "", $string); //remove GTM tag
      echo("<td>" . $string . "</td>\n");
    }

    //print cells of each row
    for ($j=0; $j < $table->ncolumns; $j++) {
      if (!is_array($table->rows[$i]->cells[$j])) {
	  echo("<td>" . $row->cells[$j] . "</td>\n");
      }
      else { //we have an array, display a drop down box
	echo("<td><select>\n");
	foreach ($row->cells[$j] as $value) {
	  echo("<option value=\"" . $value . "\">" . $value . "\n");
	}
	echo("</select></td>\n");
      }
    }

    echo("</tr>");
  }
}
    

/*prints a table*/
function printTable($table) {

  if ($table->nrows==0) {
    echo("<p>Table is empty</p>\n");
  }
  else {
    echo("<table>\n");
    
    /*header row*/
    echo("<tr class=\"header\">\n");
    echo("<td> </td>\n");
    if ($_SESSION['runDisplay']) {//display condDBKeys as run/event
      echo("<td>Since Run:</td>\n");
      echo("<td>Since Event:</td>\n");
      echo("<td>Till Run:</td>\n");
      echo("<td>Till Event:</td>\n");
    }
    else {
      echo("<td>Since (GMT):</td>\n");
      echo("<td>Till (GMT):</td>\n");
    }
    for ($i=0; $i< $table->ncolumns; $i++) //display types
      echo("<td>". $table->names[$i] . "</td>\n");
    echo("</tr>");
    
    printRows($table);
    
    echo("</table>");
  }
}

/*prints a folder*/
function printFolder($folder, $is_folderSet) {

  echo("<table>\n");
    
  /*Path*/
  echo("<tr class=\"header\">\n");
  echo("<td>Path: </td>\n");
  echo("<td> " . $folder->path . "</td>\n");
  echo("</tr>");

  //attributes
  echo("<tr class=\"even\">\n");
  echo("<td>Attributes: </td>\n");
  echo("<td> " . $folder->attributes . "</td>\n");
  echo("</tr>");

  //type
  echo("<tr class=\"odd\">\n");
  echo("<td>Type: </td>\n");

  if ($is_folderSet){ //check if is a folderSet
    echo("<td>Folder Set</td>\n");
  }
  else { //it's a folder, print real type
    switch ($folder->type) {
    case BLOBTAG:
      echo("<td>Blob with Tags</td>\n");
      break;
    case STRUCT:
      echo("<td>Cond. Table</td>\n");
      break;
    case STRUCTID:
      echo("<td>Cond. Table with IDs</td>\n");
      break;
    case BLOB:
      echo("<td>Blob</td>\n");
      break;
    case STRUCTTAG:
      echo("<td>Cond. Table with Tags</td>\n");
      break;
    case STRUCTIDTAG:
      echo("<td>Cond. Table with IDs and Tags</td>\n");
      break;
    default:
      echo("<td>Unknown</td>\n");
    }
  }
  echo("</tr>");

    //description
    echo("<tr class=\"even\">\n");
  echo("<td>Description: </td>\n");
  echo("<td> " . htmlspecialchars($folder->description) . "</td>\n");
  echo("</tr>");
  
  echo("</table>");
}

function buildWhereClause() {
  $first= true;

  //find all table options checked
  foreach ($_POST as $key => $val) {
    if ("tableCheckBox" == substr($key, 0, 13)) { //we have a checkbox in table
      $boxNum= substr($key, 13); //get the number of the checkbox
      $compareSign= $_POST["compareSign" . $boxNum]; //get sign for this ocurrence
      $fieldType= $_POST["fieldType" . $boxNum];
      $userVal= $_POST["userVal" . $boxNum]; //value entered by the user

      //get type and make preparations for the query
      switch ($fieldType) {
      //floats e ints
      case CDBINT:
      case CDBFLOAT:
      case CDBDOUBLE:
      case CDBLONGLONG:
// 	if ($compareSign== '=')
// 	  $compareSign== " LIKE ";
	break;

      case CDBBOOL:
	if ($compareSign== '=')
	  $compareSign== " LIKE ";
	$userVal= $userVal == "false" ? 0 : 1;
	break;

      case CDBSTRING:
	$userVal= "\"" . $userVal . "\"";
	break;
      }
      if ($first) { //is this the first ocurrence in the where clause?
	$first= false;
	$where= $val . " " . $compareSign  . " " . $userVal;
      }
      else
	$where.= " AND " . $val . " " . $compareSign . " " . $userVal;
    }
  }
  /*debug*/
  //  echo("Query: " . $where);
  return $where;
}

?>