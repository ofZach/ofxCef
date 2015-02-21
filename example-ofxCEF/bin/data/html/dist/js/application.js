$(document).ready(function() {
  $('#toOF').on('change', function() { 
    sendDataToOF('User input from JS', parseInt($('#toOF').val()));
  });
});

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
