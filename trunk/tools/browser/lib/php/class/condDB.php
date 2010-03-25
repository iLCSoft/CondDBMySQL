<?php
class condDB 
{
  var $handler;
  var $host;
  var $db_name;
  var $user;
  var $pass;

  function condDB($host, $db_name, $user, $pass)
  {
    $this->host= $host;
    $this->db_name= $db_name;
    $this->user= $user;
    $this->pass= $pass;

  }


  function connect() {
    /*if condDB's bind is not loaded, use dl() (OS independent)*/
    if (!extension_loaded('CondDBPHP'))
      if (!dl('conddbphp.' . PHP_SHLIB_SUFFIX))
	exit(); //file not loaded, exit

    /*open connection*/
    $this->handler= cdb_connect_open($this->host,
				     $this->db_name, $this->user, 
				     $this->pass);
    if ($this->handler == -1)
      return false;

    return true;
  }

  function getSchema($path) {
    return cdb_tbl_schema($this->handler, $path);
  }

  function getFolderSets($path) {
    return cdb_get_foldersets($this->handler, $path);
  }

  function getFolders($path) {
    return cdb_get_folders($this->handler, $path);
  }

  function getTags($path) {
    return cdb_get_tags($this->handler, $path);
  }


  function buildTreeMenu() {
    $level= ".";

    /*[dots]|[text]|[link]|[title]|[icon]|[target]|[expanded]*/
    $rootFolder= $level . "|" . "/" . "|||". FOLDERSET_IMAGE ."||1\n";
    $condFolders= $this->buildTreeMenuAux("/", $level);

    if (empty($condFolders)) { //no folders on the given database
       return '';
       }
    else {
	 return $rootFolder . $condFolders; //return tree
   }
  }

  function buildTreeMenuAux($path, $level= "") {
    $level.=".";

    $folderSets= $this->getFolderSets($path);

    $nFolderSets=  $folderSets== -1 ? 0 : count($folderSets);

    for ($i=0; $i<$nFolderSets; $i++) {
      /*[dots]|[text]|[link]|[title]|[icon]|[target]|[expanded]*/
      //       $treeString.= $level . "|" . basename(current($folderSets)) ."|||" . FOLDERSET_IMAGE . "||\n";
      /*new*/
      $treeString.= $level . "|" . basename(current($folderSets))
      . "|main_frame.php?path=";
      $treeString.=  str_replace("+", "%2b", current($folderSets)) . "&folderSet=true||" . FOLDERSET_IMAGE . "|mainFrame|\n";
      /*new ends*/
      $treeString.= $this->buildTreeMenuAux(current($folderSets), $level);
      next($folderSets);
    }

    /*get all folders on this level*/
    $folders= $this->getFolders($path);
    $nFolders= $folders== -1 ? 0 : count($folders);    


    /*[dots]|[text]|[link]|[title]|[icon]|[target]|[expanded]*/
    for ($i=0; $i<$nFolders; $i++) {
      $treeString.= $level . "|" . basename(current($folders)) . 
      	"|main_frame.php?path=" . str_replace("+", "%2b", current($folders))
      	. "&folder=true|||" . "mainFrame" . "\n";
      next($folders);
    }
    return $treeString;
  }

  /*debug remove later*/
//   function getHistory($folder, $since_year, $since_month, $since_day,
// 		      $since_hour, $since_min, $since_sec, 
// 		      $till_year, $till_month, $till_day,
// 		      $till_hour, $till_min, $till_sec, $where= '') {

//     if ($this->getFolderType($folder) == BLOBTAG) { //it's not a table
//       $found= conddbbrowsehistory($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
// 				  (int)$since_hour, (int)$since_min, (int)$since_sec, 
// 				  (int)$till_year, (int)$till_month, (int)$till_day,
// 				  (int)$till_hour, (int)$till_min, (int)$till_sec);
//       if ($found==-1)
// 	return -1;
//       return conddbprintallobj($this->handler);
//     }
//     else { //it's a table
//       if (empty($where)) { //no where clause
// 	return  $found= cdb_browse_table_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
// 						 (int)$since_hour, (int)$since_min, (int)$since_sec, 
// 						 (int)$till_year, (int)$till_month, (int)$till_day,
// 						 (int)$till_hour, (int)$till_min, (int)$till_sec);
//       }
//       else {
// 	return  $found= cdb_browse_table_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
// 						 (int)$since_hour, (int)$since_min, (int)$since_sec, 
// 						 (int)$till_year, (int)$till_month, (int)$till_day,
// 						 (int)$till_hour, (int)$till_min, (int)$till_sec, $where);
//       }
//     }
//   }
  
    function getHistory($folder, $since_year, $since_month, $since_day,
		      $since_hour, $since_min, $since_sec, 
		      $till_year, $till_month, $till_day,
		      $till_hour, $till_min, $till_sec) {

      $found= cdb_browse_obj_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
				  (int)$since_hour, (int)$since_min, (int)$since_sec, 
				  (int)$till_year, (int)$till_month, (int)$till_day,
				  (int)$till_hour, (int)$till_min, (int)$till_sec);
    if ($found==-1) /*debug error returned*/
      return -1;
    
    return cdb_print_all_obj($this->handler);
  }


  function getTableHistory($folder, $since_year, $since_month, $since_day,
			   $since_hour, $since_min, $since_sec, 
			   $till_year, $till_month, $till_day,
			   $till_hour, $till_min, $till_sec, $from=-1, $to=-1, $where= '') {
    if ($from==-1) { //no [from, to] given
      return  cdb_browse_tbl_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
				       (int)$since_hour, (int)$since_min, (int)$since_sec, 
				       (int)$till_year, (int)$till_month, (int)$till_day,
				       (int)$till_hour, (int)$till_min, (int)$till_sec);
    }

    elseif (empty($where)) { //[from, to] given, but no where clause
      return  cdb_browse_tbl_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
				       (int)$since_hour, (int)$since_min, (int)$since_sec, 
				       (int)$till_year, (int)$till_month, (int)$till_day,
				       (int)$till_hour, (int)$till_min, (int)$till_sec, $from, $to);
    }
    else { //[from, to] and where clause given
      return  cdb_browse_tbl_history($this->handler, $folder, $since_year, (int)$since_month,(int) $since_day,
				       (int)$since_hour, (int)$since_min, (int)$since_sec, 
				       (int)$till_year, (int)$till_month, (int)$till_day,
				       (int)$till_hour, (int)$till_min, (int)$till_sec, $from, $to, $where);
    }
  }

  /* This function returns objects in $folder with
   * at point represented by $year, $month, etc...
   * returns: -1 if fodler empty, else an array of
   * objects from the CondDB
   */
  function getPoint($folder, $year, $month, $day,
		    $hour, $min, $sec, $where='') 
    {
      $found= cdb_browse_obj_point($this->handler, $folder, $year, $month, $day,
					 $hour, $min, $sec);
      /*debug error found, return -1*/
      if ($found==-1)
	return -1;

      return cdb_print_all_obj($this->handler);
    }
	
  function getTablePoint($folder, $year, $month, $day,
			 $hour, $min, $sec, $from=-1, $to=-1, $where='') 
    {

      if ($from==-1) {//no [from,to] given
	return cdb_browse_tbl_point($this->handler, $folder, $year, $month, $day,
					$hour, $min, $sec);
      }
      elseif(empty($where)) { //[from, to] given, but no where clause
	return cdb_browse_tbl_point($this->handler, $folder, $year, $month, $day,
					$hour, $min, $sec, $from, $to);
      }
      else { //[from, to] and where clause given
	return cdb_browse_tbl_point($this->handler, $folder, $year, $month, $day,
					$hour, $min, $sec, $from, $to, $where);
      }
    }

  /* This function returns objects in $folder with
   * tag equal to $tag
   * returns: -1 if fodler empty, else an array of
   * objects from the CondDB
   */
  function getTag($folder, $tag='', $where= '') {

    //HEAD tag is a mnemoric to no tag
    if ($tag=="HEAD")
      $tag='';
    
    $found= cdb_browse_obj_tag($this->handler, $folder, $tag);
    if ($found==-1)
      return -1;
    return cdb_print_all_obj($this->handler);
  }

  function getTableTag($folder, $tag='', $from=-1, $to=-1, $where= '') 
    {
      //HEAD tag is a mnemoric to no tag
      if ($tag=="HEAD")
	$tag='';

      if ($from==-1) {//no [from, to] given
	return cdb_browse_tbl_tag($this->handler, $folder, $tag);
      }
	elseif (empty($where)) { //[from, to] given, but no where clause
	  return cdb_browse_tbl_tag($this->handler, $folder, $tag, $from, $to);
	}
      else { //[from, to] and where clause
	return cdb_browse_tbl_tag($this->handler, $folder, $tag, $from, $to, $where);
      }
    }

  /*return object, NULL if not found*/
  function getObject($folder, $year, $month, $day, $hour, $min, $sec, $nanosec) {
    return cdb_get_obj($this->handler, $folder, $year, $month, $day, $hour, $min, $sec, $nanosec);
  }
  
  function destroy() {
    /*debug to be made*/
    //    conddbdestroyconddbmgr();
  }

  function getFolderType($folder) {
    return cdb_get_folder_type($this->handler, $folder);
  }

  function getFolder($folder) {
    return cdb_get_folder($this->handler, $folder);
  }

}
?>
