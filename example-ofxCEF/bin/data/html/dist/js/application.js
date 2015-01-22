$(document).ready(function() {
  $('#toOF').on('change', function() { 
    app.sendDataToOF($('#toOF').val()); 
  });
});

function sendMessageToJS(strMessage) {
  $("#fromOF").val(strMessage);
}


// Some general UI pack related JS
// Extend JS String with repeat method
(function ($) {

  $(function () {
    // Focus state for append/prepend inputs
    $('.input-group').on('focus', '.form-control', function () {
      $(this).closest('.input-group, .form-group').addClass('focus');
    }).on('blur', '.form-control', function () {
      $(this).closest('.input-group, .form-group').removeClass('focus');
    });

    // make code pretty
    window.prettyPrint && prettyPrint();

  });

})(jQuery);
