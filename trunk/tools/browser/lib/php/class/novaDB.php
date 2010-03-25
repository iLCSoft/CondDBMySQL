<?php
class novaDB 
{
  var $host;
  var $user;
  var $pass;
  var $db_name;
  var $id;
  var $resource;
  var $connected;
  var $loaded;
  var $struct;
  var $bytes;
  var $bytelen;
  var $bytesize;
  var $num_rows;
  var $formatString;
  var $outputFormat;
  var $ntimes;
  var $rows;
  var $nCols;

  function novaDB($host, $db_name, $user, $pass)
  {
    $this->host= $host;
    $this->user= $user;
    $this->pass= $pass;
    $this->db_name= $db_name;
    $this->id= -1;
    $this->connected= false;
    $this->loaded= false;
  }

  /*returns true if connected, else false*/
  function connect() 
    {
      if ($this->connected)
	return true;
	
      $this->resource=mysql_connect($this->host, $this->user, $this->pass);
      
      if (!$this->resource) {
	$this->connected= false;
	return false;
      }

      $this->connected= mysql_select_db($this->db_name, $this->resource);
      return $this->connected;
    }

    function disconnect() {
      if ($this->connected) {
	$this->connected= false;
	$this->loaded= false;
	$this->id= -1;
	mysql_close($this->resource);
      }
    }

    /*loads structure into class*/
    /*returns true on success, else false*/
    function loadTable($ID) {
      if ($this->loaded && $this->$id== $ID)
	return true;

      /*get structure*/
      $this->id= $ID;
      $query= "SELECT structures.name, instances.nRows, sizeOfStruct ";
      $query.= "FROM structures, instances ";
      $query.= "WHERE instances.ID=". $this->id . " AND structures.ID=instances.strID order by structures.version";

      $result = mysql_query($query, $this->resource);
      if (!$result) {
	$this->loaded = false;
	return $this->loaded;
      }
      $this->struct= mysql_fetch_array($result);
      mysql_free_result($result);

      $bytes_result = mysql_query("SELECT bytes FROM bytes WHERE instanceID=$ID", $this->resource);
      if (!$bytes_result) {
	$this->loaded = false;
	return $this->loaded;
      }
      $this->bytes= mysql_fetch_row($bytes_result);
      $this->bytelen= mysql_fetch_lengths($bytes_result);
      $this->bytesize= $this->bytelen[0]/$this->struct["nRows"];
      mysql_free_result($bytes_result);

      /*get table*/
      $query= "SELECT headers.type,headers.name, headers.comment, headers.nDims, headers.firstDim, headers.offset ";
      $query.= "FROM headers, instances ";
      $query.= "WHERE instances.ID=$ID AND headers.strID=instances.strID ";
      $query.= "ORDER BY offset";
      $dt_result = mysql_query($query, $this->resource);
      if (!$dt_result) {
	$this->loaded = false;
	return $this->loaded;
      }


      $this->nCols = mysql_num_rows($dt_result);
      for ($i=0; $i< $this->nCols; $i++) {

	$this->rows[$i] = mysql_fetch_array($dt_result);

	if ($this->rows[$i]["nDims"]==0) {
	  $this->ntimes[]=1;
	}
	else {
	  $this->ntimes[]=$this->rows[$i]["firstDim"];
	}
      }
      mysql_free_result($dt_result);

      $this->formatString="";
      for ($i=0; $i< $this->nCols; $i++) {
	switch($this->rows[$i]["type"]) {
	case "char":  
	  $this->outputFormat[]="a";
	  break;

	case "short":
	  //are those shorts padded?
	  if ($i<$this->nCols-1)
	    $end=$this->rows[$i+1]["offset"];
	  else
	    $end=$this->struct["sizeOfStruct"];

	  $size=($end-$this->rows[$i]["offset"]);
	  if($this->ntimes[$i]==1) {
	    if ($size==2)
	      $this->outputFormat[]="s";
	    elseif ($size==4)
	      $this->outputFormat[]="i";
	    else
	      echo "ERROR: wrong short size of short found: $size";
	  }
	  else { //is this array padded?
	    $this->outputFormat[]="s";
	    $padded=$size-2*$this->ntimes[$i];
	    if ($padded==2)
	      $this->ntimes[$i]+=1; 
	    elseif ($padded!=0)
	      echo "ERROR: wrong padding size of short array is found: $padded";
	  }
	  break;

	case "long":  
	case "int":  
	  $this->outputFormat[]="i";
	  break;

	case "float":  
	  $this->outputFormat[]="f";
	  break;
	default:
	  echo "ERROR: unknown format";
	  break;
	}

	if ($this->ntimes[$i]==1)
	  $this->formatString.=$this->outputFormat[$i].$this->rows[$i]["name"];
	else
	  $this->formatString.=$this->outputFormat[$i].$this->ntimes[$i].$this->rows[$i]["name"];
	
	if ($i<$this->nCols-1) $this->formatString.="/";

	/*debug*/
// 	echo "format: $this->formatString <br>";
// 	echo "$i, name: ".$this->rows[$i]["name"].", this->ntimes: $this->ntimes[$i], count $count, format: $this->formatString<br>";

      }
      $this->loaded= true;
      return $this->loaded;

    }

    /*get data matrix from table*/
    /*returns -1 if table not loaded, else matrix*/
    /*pre: valid 'from' and 'to' intervail*/
    function getMatrix($from, $to) {
      if (!$this->loaded)
	return -1;

      $offset= $to - $from +1;

      //make data matrix with values
      for ($k=0; $k< $offset; $k++) {
	$values=unpack($this->formatString,substr($this->bytes[0],($from+$k)*$this->bytesize));
	$l=0;

	for ($i=0; $i< $this->nCols; $i++) {
	  //this is a string
	  if ($this->outputFormat[$i]=="a") $this->ntimes[$i]=1; 
	  for ($j=0; $j< $this->ntimes[$i]; $j++) {
	    $value=current($values);
	    switch($this->outputFormat[$i]) {
	    case "f":
	      if ($value==0)
		$data[$k][$l]="0";
	      else {
		$exp=intval(log10(abs($value)));
		$decimals=6-$exp;
		if ( 1<$decimals && $decimals<10 ) {
		  $string=sprintf("%.".$decimals."f",$value);
		  $pow="";
		}
		else {
		  $string=sprintf("%f",$value/pow(10,$exp));
		  if ($exp>0)
		    $pow="e+".$exp;
		  else
		    $pow="e".$exp;
		}
		//strip trailing zeroes
		$string=strtr($string,"0"," ");
		$string=chop($string);
		//strip remaining dot
		$last=strlen($string)-1;
		if (substr($string,$last,1)==".") $string=substr($string,0,$last);
		
		$data[$k][$l]=strtr($string," ","0").$pow;
		//echo strtr($string," ","0").$pow."<br>";
	      }
	      break;
	      
	    case "i":
	    case "s":
	    case "a":
	      $data[$k][$l]=$value;
	      break;
	
	    default:
	      break;
	    }
	    next($values);
	    $l++;
	  }
	}
      }
      return $data;
    }

    function getHost() {
      return $this->host;
    }

    function getUser() {
      return $this->user;
    }

    function getDBName() {
      return $this->db_name;
    }

    /*returns -1 if no ID defined*/
    function getID()
      {
	return $this->id;
      }

    function getConnected()
      {
	return $this->connected;
      }

    /*returns -1 if no struct loaded*/
    function getByteSize()
      {
	if (!$this->loaded)
	  return -1;
	return $this->bytesize;
	
      }

    /*returns -1 if no struct loaded*/
    function getByteLength()
    {   
      if (!$this->loaded)
	return -1;

      return $this->bytelen[0];
    }

    function getStructName() {
      if (!$this->loaded)
	return "";
      else
	return $this->struct["name"];
    }

    /*returns -1 if no struct loaded*/
    function getSizeOfStruct()
      {
	if (!$this->loaded)
	  return -1;

	return $this->struct["sizeOfStruct"];
      }

    /*returns -1 if no struct loaded*/
    function getNumRows()
      {
	if (!$this->loaded)
	  return -1;

	return $this->struct["nRows"];
      }

    /*return row name, -1 on error*/
    function getRowName($i) {
      if (!$this->loaded || $this->nCols < $i)
	return -1;
      else
	return $this->rows[$i]['name'];
    }

    /*return row name, -1 on error*/
    function getRowType($i) {
      if (!$this->loaded || $this->nCols < $i)
	return -1;
      else
	return $this->rows[$i]['type'];
    }

    /*return num of columns, -1 on error*/
    function getNumColumns() {
      if (!$this->loaded)
	return -1;
      else
	return $this->nCols;
    }

    /*return row name, "" on error*/
    function getRowComment($i) {
      if (!$this->loaded || $this->nCols < $i)
	return "";
      else
	return $this->rows[$i]['comment'];
    }

}
?>