/**
* A function that sets the menu cookie
* @access private
* @var string
*/

/*This file contains scripts used in the browse_menu.php*/
/*Requires date.js*/
/*Action to take when checkbuttons change*/
function refreshFormState() {

	 var form= document.forms.visualSettings;

	 switch (checkbuttons(form.visualSetting)) {
		/*Browse at point selected*/
		case "1":
		     activateBPoint(true);
		     activateBHistory(false);
		     activateBWTag(false);
			selectNext(4);
		break;
		case "2":
		     activateBPoint(false);
		     activateBHistory(true);
     		    activateBWTag(false);
			selectNext(7);
		break;
		case "3":
		     activateBPoint(false);
		     activateBHistory(false);
     		  activateBWTag(true);
		break;
		default:
		    activateBPoint(false);
		    activateBHistory(false);
		    activateBWTag(false);
	}
}


/*activate BPoint section in main_menu.php
 @param: state= true to activate, false deactivate*/

function activateBPoint(state) {

	/*constants for folder type defenition*/
	var BLOBTAG=0;
	var STRUCT=1;
	var STRUCTID=2;
	var BLOB=3;
	var STRUCTTAG=4;
	var STRUCTIDTAG=5;

	 var form= document.forms.visualSettings;

	 //do not enable browse if folder type doesn't support it
	 if (form.folderType.value==STRUCTID || form.folderType.value==STRUCT) {
	     state= false;
	     //disable radio button
	     activateItem(form.visualSetting[1], false);
	     form.visualSetting[1].checked= false;
	 }

	 activateItem(form.yearBPoint, state);
	 activateItem(form.monthBPoint, state);
 	 activateItem(form.dayBPoint, state);

 	 activateItem(form.hourBPoint, state);
 	 activateItem(form.minuteBPoint, state);
 	 activateItem(form.secondBPoint, state);
}

function activateBHistory(state) {
	 var form= document.forms.visualSettings;

	 activateItem(form.yearBHistSince, state);
	 activateItem(form.monthBHistSince, state);
 	 activateItem(form.dayBHistSince, state);

 	 activateItem(form.hourBHistSince, state);
 	 activateItem(form.minuteBHistSince, state);
 	 activateItem(form.secondBHistSince, state);

	 activateItem(form.yearBHistTill, state);
	 activateItem(form.monthBHistTill, state);
 	 activateItem(form.dayBHistTill, state);

 	 activateItem(form.hourBHistTill, state);
 	 activateItem(form.minuteBHistTill, state);
 	 activateItem(form.secondBHistTill, state);

	activateItem(form.bminutes, state);
}


function activateBWTag(state) {

	/*constants for folder type defenition*/
	var BLOBTAG=0;
	var STRUCT=1;
	var STRUCTID=2;
	var BLOB=3;
	var STRUCTTAG=4;
	var STRUCTIDTAG=5;

	 var form= document.forms.visualSettings;

	 //do not enable browse if folder type doesn't support it
	 if (form.folderType.value==STRUCTID || form.folderType.value==STRUCT) {
	     state= false;
	     //disable radio button
	     activateItem(form.visualSetting[0], false);
	     form.visualSetting[0].checked= false;
	 }

	 activateItem(form.browseWTag, state);
}


/*this function changes the state of an icon
 @param item, item to change
 @param state, state of the item
*/
function activateItem(item, state) {
	 item.disabled= !state;
}

function validateVisualSettings() {
	 var form= document.forms.visualSettings;
	 
	 switch(checkbuttons(form.visualSetting)) {
	 	case "1":
		     /*check if date is valid*/
		     if (!checkDate(form.yearBPoint.value, form.monthBPoint.value,
		         form.dayBPoint.value)) {
			 alert("Please insert a valid date.");
			 return false;
		     }
		     /*check the time*/
           if (!checkTime(form.hourBPoint.value,
					form.minuteBPoint.value, form.secondBPoint.value)) {
			 	alert("Please insert a valid time.");
			 	return false;
		     }
		     break;
		 case "2":
		   /*check "since" date and time*/
		   if (!checkDate(form.yearBHistSince.value, form.monthBHistSince.value,
		   	form.dayBHistSince.value)) {
			 alert("Please insert a valid 'since' date.");
			 return false;
		   }
			if (!checkTime(form.hourBHistSince.value,
		          form.minuteBHistSince.value, form.secondBHistSince.value)) {
			 alert("Please insert a valid 'since' time.");
			 return false;
		   }

			/*check "till" date and time*/
		   if (!checkDate(form.yearBHistTill.value, form.monthBHistTill.value,
		      	form.dayBHistTill.value)) {
				alert("Please insert a valid 'till' date.");
			 	return false;
		   }
         if (!checkTime(form.hourBHistTill.value,
		   		form.minuteBHistTill.value, form.secondBHistTill.value)) {
				alert("Please insert a valid 'till' time.");
				return false;
		   }

	     /*check if "since" date is smaller that "till" date*/
	     var sinceDate= new Date(form.yearBHistSince.value, form.monthBHistSince.value,
          form.dayBHistSince.value, form.hourBHistSince.value,
          form.minuteBHistSince.value, form.secondBHistSince.value);
	     var tillDate= new Date(form.yearBHistTill.value, form.monthBHistTill.value,
          form.dayBHistTill.value, form.hourBHistTill.value,
          form.minuteBHistTill.value,form.secondBHistTill.value);

			if( tillDate.getTime() <= sinceDate.getTime()) {
			 	alert("'till' date must be bigger that 'since' date.");
			 return false;
			}

	      break;
		 case "3":
		 /*Nothing to do yet...*/		 
		 break;
		 default:
			alert("Must select an option on the Browse Menu.");
			return false;
	 }
	 return true;
}
/*Selects to the next object*/
function selectNext(nextId)
{
	document.forms.visualSettings.elements[nextId].select();
	document.forms.visualSettings.elements[nextId].focus();
}

/*Selects to the next object if maxlength was reached for this textbox*/
function nextElement(currElem, nextId)
{
	 var form= document.forms.visualSettings;
	 
	if (currElem.value.length==currElem.maxLength) {
			form.elements[nextId].select();
			form.elements[nextId].focus();
	}
}

/*Need to put these functions in a lib*/

/*returns the value of the selected button*/
/*@param group: radiobutton group class object*/
function checkbuttons(group) {
	 for (var i = 0; i < group.length; i ++) {
	     if (group[i].checked) return group[i].value;
       }
  return false;
}

/*returns the value of the selected item in the list*/
/*@param dropdown: select class object*/
/*Pre: Must not be a "select-multiple" type of select*/
function selectValue(dropdown) {
	 return dropdown.options[dropdown.selectedIndex].value;
}

function bMinutesTrigger() 
{
	 var form= document.forms.visualSettings;

	var till= new Date();
	var since= new Date();

	 /*subtract selected minutes*/
	var delta=till.getMinutes() - form.bminutes.options[form.bminutes.selectedIndex].text;
	since.setUTCMinutes(delta);

	/*set since date*/
	form.yearBHistSince.value= since.getUTCFullYear();
	form.monthBHistSince.value= since.getUTCMonth()+1;
	form.dayBHistSince.value= since.getUTCDate();

	form.hourBHistSince.value= since.getUTCHours();
	form.minuteBHistSince.value= since.getUTCMinutes();
	form.secondBHistSince.value= since.getUTCSeconds();

	/*set till date*/
	form.yearBHistTill.value= till.getUTCFullYear();
	form.monthBHistTill.value= till.getUTCMonth()+1;
	form.dayBHistTill.value= till.getUTCDate();

	form.hourBHistTill.value= till.getUTCHours();
	form.minuteBHistTill.value= till.getUTCMinutes();
	form.secondBHistTill.value= till.getUTCSeconds();
}

