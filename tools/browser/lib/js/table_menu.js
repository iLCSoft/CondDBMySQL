/*This file contains scripts used in the table_menu.php*/
/*Requires date.js*/
/*Requires browser_menu.js*/

function validateTableMenu() {

	var vSettings= document.forms.visualSettings;
	var tMenu= document.forms.tableMenu;

	//see if values on the first menu are valid
	if(validateVisualSettings() == false)
		return false;

	//copy values from the first menu to the hidden in the table_menu
	tMenu.visualSetting.value= checkbuttons(vSettings.visualSetting);

	//run display flag
	if (vSettings.runDisplay.checked)
		tMenu.runDisplay.value= vSettings.runDisplay.value;

	//browse with tag
	tMenu.browseWTag.value= vSettings.browseWTag.value;
	//browse at point
	tMenu.yearBPoint.value= vSettings.yearBPoint.value;
	tMenu.monthBPoint.value= vSettings.monthBPoint.value;
	tMenu.dayBPoint.value= vSettings.dayBPoint.value;
	tMenu.hourBPoint.value= vSettings.hourBPoint.value;
	tMenu.minuteBPoint.value= vSettings.minuteBPoint.value;
	tMenu.secondBPoint.value= vSettings.secondBPoint.value;
	//browse history
	tMenu.yearBHistSince.value= vSettings.yearBHistSince.value;
	tMenu.monthBHistSince.value= vSettings.monthBHistSince.value;
	tMenu.dayBHistSince.value= vSettings.dayBHistSince.value;
	tMenu.hourBHistSince.value= vSettings.hourBHistSince.value;
	tMenu.minuteBHistSince.value= vSettings.minuteBHistSince.value;
	tMenu.secondBHistSince.value= vSettings.secondBHistSince.value;
	tMenu.yearBHistTill.value= vSettings.yearBHistTill.value;
	tMenu.monthBHistTill.value= vSettings.monthBHistTill.value;
	tMenu.dayBHistTill.value= vSettings.dayBHistTill.value;
	tMenu.hourBHistTill.value= vSettings.hourBHistTill.value;
	tMenu.minuteBHistTill.value= vSettings.minuteBHistTill.value;
	tMenu.secondBHistTill.value= vSettings.secondBHistTill.value;

	//folder type
	tMenu.folderType.value= vSettings.folderType.value;
	return true;
}

function switchEnableGroup(i) {
	var tMenu= document.forms.tableMenu;
	
	var compareName= "compareSign" + i;
	var userVal= "userVal" + i;

	for (var j=0; j<tMenu.elements.length; j++) {
		if (tMenu.elements[j].name==  userVal) {
			tMenu.elements[j].disabled= !tMenu.elements[j].disabled;
		}
		else if (tMenu.elements[j].name==  compareName) {
			tMenu.elements[j].disabled= !tMenu.elements[j].disabled;
		}
	}
}