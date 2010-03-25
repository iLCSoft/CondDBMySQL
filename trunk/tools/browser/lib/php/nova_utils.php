<?php

/*prints description*/
/*returns false if error printed*/
function nova_print_description($ID, &$novaDB) {

   if (!$novaDB->loadTable($ID)) {
    echo("Unable to load table with ID=$ID");
    return false;
  }
  else {
    echo("<table>\n");

    /*payload server*/
    echo($desc.="<tr class=\"header\">\n");
    echo("<td>Payload Server: </td>\n");
    echo("<td> " . $novaDB->getHost() . "</td>\n");
    echo("</tr>");

    /*payload DB*/
    echo("<tr class=\"even\">\n");
    echo("<td>Payload DB: </td>\n");
    echo("<td> " . $novaDB->getDBName() . "</td>\n");
    echo("</tr>");

  /*payload ref*/
    echo("<tr class=\"odd\">\n");
    echo("<td>Payload Ref: </td>\n");
    echo("<td> " . $novaDB->getID() . "</td>\n");
    echo("</tr>");

    echo("<tr class=\"even\">\n");
    echo("<td>Structure: </td>\n");
    echo("<td> " . $novaDB->getStructName() . "</td>\n");
    echo("</tr>");

    /*Ocurrences*/
    echo("<tr class=\"odd\">\n");
    echo("<td>Number of Structs: </td>\n");
    echo("<td> " . $novaDB->getNumRows() . "</td>\n");
    echo("</tr>");

    echo("<tr class=\"even\">\n");
    echo("<td>Byte Length: </td>\n");
    echo("<td> " . $novaDB->getByteLength() . "</td>\n");
    echo("</tr>");

    echo("</table>");

    return true;
  }
}

/*prints table*/
/*returns false if error printed*/

function nova_print_table(&$novaDB) {
  require_once(PHP_LIB_PATH . "utils.php");
  $interval= getInterval($_GET['from'], $_GET['to'], $novaDB->getNumRows());

  $matrix= $novaDB->getMatrix($interval['from'], $interval['to']);

  if ($matrix==-1) {
    echo("Table in unknow format");
    return false;
  }

  //printing table
  if ($novaDB->getNumRows()==0) {
    echo("<p>Table is empty</p>\n");
  }
  else {
    echo("<table>\n");
    
    /*header rows*/
    /*print comments*/
    echo("<tr class=\"header\">\n");
    echo("<td>Comments: </td>\n");
    for ($i=0; $i< $novaDB->getNumColumns(); $i++) {
      echo("<td>" . $novaDB->getRowComment($i). "</td>\n");
    }
    echo("</tr>\n");

    /*debug*/
//     /*print types*/
//     echo("<tr class=\"header\">\n");
//     echo("<td>Type: </td>\n");
//     for ($i=0; $i< $novaDB->getNumColumns(); $i++) {
//       echo("<td>" . $novaDB->getRowType($i). "</td>\n");
//     }
//     echo("</tr>\n");

    /*print names*/
    echo("<tr class=\"header\">\n");
    echo("<td>Name: </td>\n");
    for ($i=0; $i< $novaDB->getNumColumns(); $i++) {
      echo("<td>" . $novaDB->getRowName($i). " (" . $novaDB->getRowType($i) . ")</td>\n");
    }
    echo("</tr>\n");

    /*data*/
    for ($i=0; $i< $interval['offset']; $i++) {
      if ( $i%2 == 0 ) { //check if it is an even or odd line
	echo("<tr class=\"even\">\n");
      }
      else {
	echo("<tr class=\"odd\">\n");
      }
      echo("<td>[" . ($interval['from'] + $i) . "]</td>\n");
      
      /*note that all values are in ONE array, even if one CELL is an array struct in NOVA*/
      for ($j=0, $k=0; $j< $novaDB->getNumColumns(); $j++, $k++) {
	if ($novaDB->ntimes[$j] == 1) {
	  echo("<td>" . $matrix[$i][$k] . "</td>\n");
	}
	else { //we have an array, display a drop down box
	  echo("<td><select>\n");
	  for($w=0; $w < $novaDB->ntimes[$j]; $w++, $k++) {
	    echo("<option value=\"" . $matrix[$i][$k] . "\">" . $matrix[$i][$k] . "\n");
	  }
	  echo("</select></td>\n");
	}
      }
      echo("</tr>\n");
    }

    /*print names*/
    echo("<tr class=\"header\">\n");
    echo("<td>Name: </td>\n");
    for ($i=0; $i< $novaDB->getNumColumns(); $i++) {
      echo("<td>" . $novaDB->getRowName($i). " (" . $novaDB->getRowType($i) . ")</td>\n");
    }
    echo("</tr>\n");


    echo("</table>\n");

  }
  return true;
}

/*debug*/
// function nova_print_table($ID, $nRows, $sizeOfStruct, $bytesize, $bytes) {
//   $query= "SELECT headers.type,headers.name, headers.comment, headers.nDims, headers.firstDim, headers.offset ";
//   $query.= "FROM headers, instances ";
//   $query.= "WHERE instances.ID=$ID AND headers.strID=instances.strID ";
//   $query.= "ORDER BY offset";

//   $dt_result = mysql_query($query);
//   if (!$dt_result)
//     die("Invalid query to Nova");

//   $num_rows = mysql_num_rows($dt_result);

//   for ($i=0; $i<$num_rows; $i++) {
//     $row[$i] = mysql_fetch_array($dt_result);

//     if ($row[$i]["nDims"]==0) {
//       $nTimes[]=1;
//     }
//     else {
//       $nTimes[]=$row[$i]["firstDim"];
//     }
//   }
//   $formatString="";
//   for ($i=0; $i<$num_rows; $i++) {
//     switch($row[$i]["type"]) {
//     case "char":  
//       $outputFormat[]="a";
//       break;

//     case "short":
//       //are those shorts padded?
//       if ($i<$num_rows-1)
// 	$end=$row[$i+1]["offset"];
//       else
// 	$end=$sizeOfStruct;

//       $size=($end-$row[$i]["offset"]);
//       if($nTimes[$i]==1) {
// 	if ($size==2)
// 	  $outputFormat[]="s";
// 	elseif ($size==4)
// 	  $outputFormat[]="i";
// 	else
// 	  echo "ERROR: wrong short size of short found: $size";
//       }
//       else { //is this array padded?
// 	$outputFormat[]="s";
// 	$padded=$size-2*$nTimes[$i];
// 	if ($padded==2)
// 	  $nTimes[$i]+=1; 
// 	elseif ($padded!=0)
// 	  echo "ERROR: wrong padding size of short array is found: $padded";
//       }
//       break;

//     case "long":  
//     case "int":  
//       $outputFormat[]="i";
//       break;

//     case "float":  
//       $outputFormat[]="f";
//       break;
//     default:
//       echo "ERROR: unknown format";
//       break;
//     }

//     if ($nTimes[$i]==1)
//       $formatString.=$outputFormat[$i].$row[$i]["name"];
//     else
//       $formatString.=$outputFormat[$i].$nTimes[$i].$row[$i]["name"];
    
//     if ($i<$num_rows-1) $formatString.="/";

// //     echo "format: $formatString <br>";
// //     echo "$i, name: ".$row[$i]["name"].", nTimes: $nTimes[$i], count $count, format: $formatString$<br>";
//   }
  
//   /*get interval of rows to display*/
//   require_once(PHP_LIB_PATH . "utils.php");
//   $interval= getInterval($nRows);

//   //make data matrix with values
//   for ($k=0; $k<$interval['offset']; $k++) {
//     $values=unpack($formatString,substr($bytes[0],($interval['from']+$k)*$bytesize));
//     $l=0;
//     for ($i=0; $i<$num_rows; $i++) {
//       //this is a string
//       if ($outputFormat[$i]=="a") $nTimes[$i]=1; 
//       for ($j=0; $j<$nTimes[$i]; $j++) {
// 	$value=current($values);
// 	switch($outputFormat[$i]) {
// 	case "f":
// 	  if ($value==0)
// 	    $data[$k][$l]="0";
// 	  else {
// 	    $exp=intval(log10(abs($value)));
// 	    $decimals=6-$exp;
// 	    if ( 1<$decimals && $decimals<10 ) {
// 	      $string=sprintf("%.".$decimals."f",$value);
// 	      $pow="";
// 	    }
// 	    else {
// 	      $string=sprintf("%f",$value/pow(10,$exp));
// 	      if ($exp>0)
// 		$pow="e+".$exp;
// 	      else
// 		$pow="e".$exp;
// 	    }
// 	    //strip trailing zeroes
// 	    $string=strtr($string,"0"," ");
// 	    $string=chop($string);
// 	    //strip remaining dot
// 	    $last=strlen($string)-1;
// 	    if (substr($string,$last,1)==".") $string=substr($string,0,$last);
	    
// 	    $data[$k][$l]=strtr($string," ","0").$pow;
// 	    //echo strtr($string," ","0").$pow."<br>";
// 	  }
// 	  break;
	  
// 	case "i":
// 	case "s":
// 	case "a":
// 	  $data[$k][$l]=$value;
// 	  break;
	
// 	default:
// 	  break;
// 	}
// 	next($values);
// 	$l++;
//       }
//     }
//   }
  
//   print_r($data);
// }

?>

