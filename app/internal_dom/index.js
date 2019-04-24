
function $(id) {
  return document.getElementById(id);
}

function onLoad(event) {
  $('message-button').addEventListener('click', onMessageButton);
}

function onMessageButton(e) {
  window.cefQuery({
    request: 'MessageRouterTest:' + document.getElementById("message").value,
    onSuccess: function(response) {
      document.getElementById('message-result').value = 'Response: '+response;
    },
    onFailure: function(error_code, error_message) {}
  });
}

document.addEventListener('DOMContentLoaded', onLoad);
