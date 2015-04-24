function sendDataToOF(name, val)
{
  app.sendMessageToOF(name, val); 
}

function sendStringToOF(name, val)
{
  app.sendMessageWithTypeToOF('string', name, val); 
}

function sendDoubleToOF(name, val)
{
  app.sendMessageWithTypeToOF('double', name, val); 
}

function sendIntToOF(name, val)
{
  app.sendMessageWithTypeToOF('int', name, val); 
}

function sendMessageToJS(strMessage)
{
  $("#fromOF").val(strMessage);
}
