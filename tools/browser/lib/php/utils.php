<?php
/*some functions of general utility*/
/*requires defines.php*/

/*this procedure prepares the environment of the browser*/
/*is used after each request*/
/*pre: Connection to the conditions was already made*/
function build_env($condDB) {
  //check if the state as changed
  if ($_GET['folderSet']) { //browsing a folderset
    $_SESSION['browser_state']= S_FOLDER_SET;
    $_SESSION['folder']= $_GET['path']; //save working folder
    return;
  }
  if ($_GET['folder']) { //browsing a folder
    $_SESSION['browser_state']= S_FOLDER;
    $_SESSION['folder']= $_GET['path']; //save working folder
    return;
  }

  //browsing blobs or tables folder?
  if (isset($_POST['visualSetting']) ) { //browser menu was used, so we already have a folder

    //register display mode
    $_SESSION['runDisplay']= ($_POST['runDisplay']=="on") ? true : false;

    //     unset($_SESSION['cond_table_where_clause']); //clean where clause /*debug remove later*/
    $browse_type=register_browse_times(); //register browse times
    $_SESSION['from']= 0; //set [from, to]
    $_SESSION['to']= MAX_INTERVAL-1;
    //check what kind of folder we have
    switch($condDB->getFolderType($_SESSION['folder'])) {
      /*BLOBS*/
    case BLOBTAG: //folder containing blobs
      switch($browse_type) { //check what kind of browse
      case B_TAG:
	$_SESSION['tag']= $_POST['browseWTag'];
	$_SESSION['browser_state']= S_BLOB_TAG;
	break;
      case B_HISTORY:
	$_SESSION['browser_state']= S_BLOB_HISTORY;
	break;
      case B_POINT:
	$_SESSION['browser_state']= S_BLOB_POINT;
	break;
      default:
	$_SESSION['browser_state']= S_UNKNOWN;
      }
      break;

      /*tables*/
      case STRUCT:
      case STRUCTID:
      case STRUCTTAG:
	switch($browse_type) { //check what kind of browse
	case B_TAG:
	  $_SESSION['tag']= $_POST['browseWTag'];
	  $_SESSION['browser_state']= S_TABLE_TAG;
	  break;
	case B_HISTORY:
	  $_SESSION['browser_state']= S_TABLE_HISTORY;
	  break;
	case B_POINT:
	  $_SESSION['browser_state']= S_TABLE_POINT;
	  break;
	default:
	  $_SESSION['browser_state']= S_UNKNOWN;
	}
	break;
    default:
      die("Error! Invalid menu option.");
    }
    return;
  }

  //browsing a blob detail?
  if ($_GET['obj_detail']== true) {
    $_SESSION['browser_state']= S_BLOB_DETAIL;
    return;
  }

 /*browsing a NOVA Database*/
 if ($_GET['nova']) {
   $_SESSION['browser_state']= S_NOVA;
   $_SESSION['from']=requestVar("from");
   $_SESSION['to']=requestVar("to");
   $_SESSION['nova_host']= DB_HOST;
   $_SESSION['nova_db']= $_GET['novaDB'];
   $_SESSION['nova_user']= DB_USER;
   $_SESSION['nova_pass']= DB_PASS;
   $_SESSION['nova_id']= requestVar("ID");
   return;
 }

 /*get state directly from link*/
 /*this will be the final pattern*/
 $browser_state= requestVar("browser_state");
 if ( $browser_state ) { /*debug if() will be removed in the final version*/

   $_SESSION['browser_state']= $browser_state;

   /*put swicth here*/
   /*get [from, to]*/
   $from= requestVar("from");
   if ($from>=0) {
     $_SESSION['from']= $from;
     $_SESSION['to']= requestVar("to");
   }
   return;
 }

 //browser starting
 $_SESSION['browser_state']= S_START;
 return;
}

/*auxiliar function to build_env()*/
/*this function registers the options of the menu (times, queries...)*/
/*returns what kind of browsing was selected (see defines.php)*/
function register_browse_times() {
  $_SESSION['visualSetting']= $_POST['visualSetting']; //save current state

  switch($_SESSION['visualSetting'])
    {
    case B_TAG: 
      $_SESSION['tag']= $_POST['browseWTag'];
      break;
    case B_HISTORY:
      /*since*/
      $_SESSION['yearSince']= $_POST['yearBHistSince'];
      $_SESSION['monthSince']= $_POST['monthBHistSince'];
      $_SESSION['daySince']= $_POST['dayBHistSince'];
      $_SESSION['hourSince']= $_POST['hourBHistSince'];
      $_SESSION['minuteSince']= $_POST['minuteBHistSince'];
      $_SESSION['secondSince']= $_POST['secondBHistSince'];
      /*till*/
      $_SESSION['yearTill']= $_POST['yearBHistTill'];
      $_SESSION['monthTill']= $_POST['monthBHistTill'];
      $_SESSION['dayTill']= $_POST['dayBHistTill'];
      $_SESSION['hourTill']= $_POST['hourBHistTill'];
      $_SESSION['minuteTill']= $_POST['minuteBHistTill'];
      $_SESSION['secondTill']= $_POST['secondBHistTill'];
      break;
    case B_POINT:
      $_SESSION['year']= $_POST['yearBPoint'];
      $_SESSION['month']= $_POST['monthBPoint'];
      $_SESSION['day']= $_POST['dayBPoint'];
      $_SESSION['hour']= $_POST['hourBPoint'];
      $_SESSION['minute']= $_POST['minuteBPoint'];
      $_SESSION['second']= $_POST['secondBPoint'];
      break;
    default:
      die("Error! Invalid menu option.");
    }
  //register where clause
  require_once(PHP_LIB_PATH . "cond_utils.php");
  $_SESSION['cond_table_where_clause']=buildWhereClause();
  return $_SESSION['visualSetting'];
}

/*this procedure computes the request*/
/*it looks at the browser state and computes the request*/
/*$condDB is an instance of the condDB class*/
function dispatch_http_request($condDB) {
  switch($_SESSION['browser_state']) {

  case S_START: //starting browser
    require_once(PHP_LIB_PATH . "browser_description.php");
    break;

  case S_FOLDER: //browsing folder
    require_once(PHP_LIB_PATH . "browse_folder.php");
    break;

  case S_FOLDER_SET: //browsing folder set
    require_once(PHP_LIB_PATH . "browse_folder_set.php");
    break;

  case S_BLOB_TAG: //browsing blob with tag
    require_once(PHP_LIB_PATH . "browse_tag.php");
    break;

  case S_BLOB_POINT: //browsing blob at point
    require_once(PHP_LIB_PATH . "browse_point.php");
    break;
  case S_BLOB_HISTORY: //browsing blob history
    require_once(PHP_LIB_PATH . "browse_history.php");
    break;
  case S_BLOB_DETAIL: //browsing blob detail
    require_once(PHP_LIB_PATH . "browse_object.php");
    break;
  case S_TABLE_TAG: //browsing table with tag
    require_once(PHP_LIB_PATH . "browse_table_tag.php");
    break;
  case S_TABLE_POINT: //browsing table at point
    require_once(PHP_LIB_PATH . "browse_table_point.php");
    break;
  case S_TABLE_HISTORY: //browsing table history
    require_once(PHP_LIB_PATH . "browse_table_history.php");
    break;
  case S_NOVA: //browsing NOVA
    require_once(CLASS_PATH . "novaDB.php");
    $novaDB= new novaDB($_SESSION['nova_host'], $_SESSION['nova_db'], $_SESSION['nova_user'], $_SESSION['nova_pass']);
     require_once(PHP_LIB_PATH . "browse_nova.php");
    break;
  case S_UNKNOWN: //unknow state
    die("Unknown action to take. Please contact technical support.");
    break;
  default:
    die("Unknown action to take. Please make sure that the browser environment was prepared.");
    break;
  }
}

/*returns intervail of rows to display*/
/*intervail is an array with 'from' and 'to'*/
/*also with an offset= 'to' - 'from'*/
/*$numRows is the number of rows of the table, 
/* if equals -1, then return the default interval*/
function getInterval($from, $to, $numRows= -1)
{

  if ($num_rows== -1) {
    $interval['from'] = 0;
    $interval['to']= MAX_INTERVAL -1;
    $interval['offset']= $interval['to'] - $interval['from'] +1;
    return $interval;
  }

  //set from
  if ($from < 0 || 
      $from  > $numRows-1) {
    $interval['from']=0;
  }
  else
    $interval['from'] = $from;

  //set to
  if ( $interval['from'] > $to ||
       $to > $numRows-1 ||
       $from==0 ) {
    if ( ($interval['from'] + MAX_INTERVAL-1) > $numRows-1)
      $interval['to']= $numRows-1;
    else
      $interval['to']= $interval['from'] + MAX_INTERVAL - 1;
  }
  else
    $interval['to']= $to;

  $interval['offset']= $interval['to'] - $interval['from'] +1;
  return $interval;
}

/*displays the footer that interacts between displays of the same table*/
function print_index_footer($url, $from, $to, $max_rows) {
  //get current interval
  $interval=getInterval($from, $to, $max_rows);

  //nothing to do  
  if ($interval['from']==0 && $interval['to']== $max_rows-1)
    return "";

  $output_string="\n<table><tr>\n";

  if ($interval['offset'] < MAX_INTERVAL) { //we are at the end
        $back_num=print_index_backward($url, $interval['from'] - MAX_INTERVAL, $interval['from']-1, $max_rows, INDEX_LIMIT/2, $output_string);
	//print current index
	$curr_index= $interval['from'] / MAX_INTERVAL + 1;
	$output_string.= "<td>&nbsp " . $curr_index . " &nbsp</td>\n";
	$output_string.="</tr></table>\n";
	return $output_string;
  }

  if ($interval['from'] > 0)
    $back_num=print_index_backward($url, $interval['from'] - MAX_INTERVAL, $interval['to'] - MAX_INTERVAL, $max_rows, INDEX_LIMIT/2, $output_string);

  //print current index
  $curr_index= $interval['from'] / MAX_INTERVAL + 1;
  $output_string.="<td>&nbsp " . $curr_index . " &nbsp</td>\n";

  if ($interval['to'] < $max_rows-1) {
    print_index_forward($url, $interval['from'] + MAX_INTERVAL, $interval['to'] + MAX_INTERVAL, $max_rows, INDEX_LIMIT -1 -$back_num, $output_string);
  }

  $output_string.="</tr></table>\n";

  return $output_string;
}

function print_index_backward($url, $from, $to, $max_rows, $iteractions, &$output_string)
{

  if ($from+MAX_INTERVAL-1<=0) //we reached the limit
    return 0;

  $interval= getInterval($from, $to, $max_rows);

  if($iteractions==0) { //stop recrusive and print previous
    $prev_from= max(0, $interval['from'] - (MAX_INTERVAL*5));
    $prev_to= $prev_from + MAX_INTERVAL-1;
    if(strchr($url, '?'))
      $curr_url= $url . "&from=" . $prev_from . "&to=" . $prev_to;
    else
      $curr_url= $url . "?from=" . $prev_from . "&to=" . $prev_to;
    echo("<p>Estou aqui</p>");
    $output_string.="<td> <a href=" . $curr_url . ">prev</a></td>\n";
    return 0;
  }

  //call recursive
  $back_num= print_index_backward($url, $interval['from'] - MAX_INTERVAL, $interval['to'] - MAX_INTERVAL, $max_rows, $iteractions-1, $output_string);
  //print link
  if(strchr($url, '?'))
    $curr_url= $url . "&from=" . $interval['from'] . "&to=" . $interval['to'];
  else
    $curr_url= $url . "?from=" . $interval['from'] . "&to=" . $interval['to'];

  $curr_index= $interval['from'] / MAX_INTERVAL + 1;
  
  $output_string.="<td> <a href=" . $curr_url . ">" . $curr_index . "</a></td> \n";

  return $back_num + 1;
  
}

function print_index_forward($url, $from, $to, $max_rows, $iteractions, &$output_string)
{

  if (($to-MAX_INTERVAL)>=$max_rows-1) { //we reached the limit
    return 0;
  }
    
  $interval= getInterval($from, $to, $max_rows);

  if($iteractions==0) { //stop recrusive and print next
//     $next_from= min($max_rows, $interval['from'] + (MAX_INTERVAL*5));
    $next_from= min($max_rows, $interval['from'] + (MAX_INTERVAL*5));
    $next_from= $next_from - ($next_from % MAX_INTERVAL);
    $next_to= min($max_rows, $interval['to'] + (MAX_INTERVAL*5));
    if(strchr($url, '?'))
      $curr_url= $url . "&from=" . $next_from . "&to=" . $next_to;
    else
      $curr_url= $url . "?from=" . $next_from . "&to=" . $next_to;
    $output_string.="<td> <a href=" . $curr_url . ">next</a></td>\n";
    return 0;
  }


  //print link
  if(strchr($url, '?'))
    $curr_url= $url . "&from=" . $interval['from'] . "&to=" . $interval['to'];
  else
    $curr_url= $url . "?from=" . $interval['from'] . "&to=" . $interval['to'];

  $curr_index= $interval['from'] / MAX_INTERVAL + 1;
  
  $output_string.="<td> <a href=" . $curr_url . ">" . $curr_index . "</a></td>\n";

  //call recursive
  /*debug*/
//   if ($interval['to'] < $max_rows -1) { //this is not the last
    $forward_num= print_index_forward($url, ($interval['from'] + MAX_INTERVAL), ($interval['to'] + MAX_INTERVAL), $max_rows, $iteractions-1, $output_string);
    return $forward_num + 1;
    /*debug*/
//   }
//   return 1;
  
}

/*return a var from $_POST or $_GET*/
function requestVar($name) {
  $val = ( isset($_POST[$name]) ) ? $_POST[$name] : ( isset($_GET[$name]) ) ? $_GET[$name] : false;
  if ( !$val ) {
    return false;
  } else {
    return addslashes(htmlspecialchars(trim($val)));
  }
}

?>