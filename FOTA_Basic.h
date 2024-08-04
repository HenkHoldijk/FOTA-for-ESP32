/* ----------------------------------------------------------------------------

FOTA_Basic.h : 'Firmware Over The Air' upload sketches remotely.

  FOTA_Basic.h holds 4 variables an 1 PROGMEM for FOTA.ino

  Basic web GUI, you may a create better looking one like FOTA.h

---------------------------------------------------------------------------- */

// ----- Some global variables

String FOTA_user;
String FOTA_password;

unsigned long FOTA_bytes_written;  // only used to log end message
String FOTA_update_error;          // we may need to return an error code

// ====== Upload interface

const char FOTA_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<meta name='viewport' content='width=device-width, height=device-height, initial-scale=1, user-scalable=no'>
<title>Firmware Over The Air</title>
</head>
<body style='background-color: #E6E6FA;'>
<center>
<div id="FOTA" style="display:block;">
    <h1>Firmware Over The Air</h1>
  <button type="button" onclick="history.back()">Back</button><br><br>
  <form onsubmit="event.preventDefault(); uploadFile();">
    <input type="file" id="firmwareFile" name="firmwareFile" required onchange="updateSelectedFileName()">
    <br><br><button type="submit">Upload Firmware</button>
  </form>
  <br>
  <div id="progressText">No Firmware Loaded</div>
  <progress id="progressBar" value="0" max="100" style="width:300px;"></progress>
  <br><br>
  <button id="rebootButton" style="display: none;" onclick="reboot()" >Reboot</button>
</div>
<div id="REBOOT" style="display:none;">
<h1>
Firmware Upload Complete<br><br>
Wait <span id="countdown">25</span> seconds for reboot to finish...
</h1>
</div>
</div>
</center>
<script>

// ----- Short for document.getElementById function

function _(el) {
  return document.getElementById(el);
}

// ----- Function to update display on selected file

function updateSelectedFileName() {
  _('progressText').innerText = 'Ready for upload';
}

// ----- Upload function

function uploadFile() {
  const fileInput = _('firmwareFile');
  const file = fileInput.files[0];

  const xhr = new XMLHttpRequest();
  const formData = new FormData();
  formData.append('file', file);

  xhr.upload.addEventListener('progress', function(e) {
    if (e.lengthComputable) {
      const percentComplete = (e.loaded / e.total) * 100;
      _('progressBar').value = percentComplete;
      _('progressText').innerText = "Loaded "+percentComplete.toFixed(2) + '%; Upload in progress....';
    }
  });

  xhr.upload.addEventListener('load', function(e) {
    _('progressText').innerText = 'Upload complete';
    _("rebootButton").style.display = "block";
  });

  xhr.addEventListener('load', function(e) {
    if (xhr.status === 200) {
      alert('You may reboot now');
    } else {
      alert('Firmware update failed');
    }
  });

  xhr.addEventListener('error', function(e) {
    alert('An error occurred while uploading the file\n\nYou may retry');
  });

  xhr.open('POST', '/FOTA/upload');
  xhr.send(formData);
}

// ----- Reboot function

function reboot() {
  fetch("/FOTA/reboot")
  _('FOTA').style.display="none";
  _('REBOOT').style.display="block";
  var timeLeft = 25;
  var countdownElement = document.getElementById("countdown");
  countdownElement.innerHTML = timeLeft;
  var countdownTimer = setInterval(function() {
    timeLeft--;
    countdownElement.innerHTML = timeLeft;
    if (timeLeft <= 0) {
      clearInterval(countdownTimer);
      window.location.href = '/';
    }
  }, 1000);

}
</script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------