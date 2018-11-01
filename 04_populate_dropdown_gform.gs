var sheetId = 'YOUR_SHEET_ID';
var sheetName = 'sheet_1';
var column = 'C';
var formId = 'YOUR_FORM_ID';
var listItemId = 12345678;

// To find listItemId uncomment this and check Log
//var form = FormApp.getActiveForm();
//var items = form.getItems();
//for (var i in items) { 
//  Logger.log(items[i].getTitle() + ': ' + items[i].getId());
//}

function spreadSheetGetter() {
  var sheet = SpreadsheetApp.openById(sheetId).getSheetByName(sheetName);
  var rows = sheet.getDataRange();
  var numRows = rows.getNumRows();
  var docId = sheet.getRange(sheetName+"!"+column+"2:"+column+numRows).getValues();
  fillFormData([].concat.apply([],docId));
}

function fillFormData(docId) {
  var form = FormApp.openById(formId);
  var item = form.getItemById(listItemId).asListItem();
  item.setChoiceValues(docId);
};
