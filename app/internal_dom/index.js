
function $(id) {
  return document.getElementById(id);
}

function onLoad(event) {
  $('sample-button').addEventListener('click', onSampleButton);
}

function onSampleButton(e) {
  $('sample-content').textContent = 'You get it!';
}

document.addEventListener('DOMContentLoaded', onLoad);
