/* ----------------------------------------------------------------------------

FOTA_Cloud.h : 'Firmware Over The Air' upload sketches remotely.

  FOTA_Cloud.h holds 4 variables an 1 PROGMEM for FOTA.ino
    Nice buttons
    Moving golden cloud with sun
    
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
<style>
  :root {
   --scale-factor: 1;
  }
  body {
   background-color: #E6E6FA;
   margin: 0;
   padding: 0;
   height: 100vh;
   display: flex;
   justify-content: center;
//   align-items: center; // center page vertically or not
   overflow: hidden;
  }
  .container {
   text-align: center;
   transform-origin: top;
   transform: scale(var(--scale-factor));
  }
  .scrollable {
   height: auto;
   display: block;
   overflow: auto;
  }
  .rim-only-button {
    width: 250px;
    height: 56px;
    background-color: lightgrey;
    border-width:3px;
    border-color:gold;
    border-radius: 50%;
    color: blue;
    text-align: center;
    font-size: 16px;
  }
  .file-input-wrapper {
    background-color: transparent;
    border-radius: 20px;
    width: 250px;
    height: 50px;
    top: -47px;
    position: relative;
    overflow: hidden;
    display: inline-block;
  }
  .file-input {
    font-size: 100px;
    position: absolute;
    left: 0;
    top: 0;
    opacity: 0;
  }
  .file-label {
    background-color: transparent;
    color: blue;
    font-size: 16px;
    cursor: pointer;
    display: inline-block;
    padding: 10px 20px;
  }
  .back_btn, .upload_btn, .reboot_btn {
    background-color: lightgrey;
    border-width:3px;
    border-color:gold;
    border-radius: 50%;
    color: blue;
    text-align: center;
    font-size: 16px;
  }
  .back_btn {
    width: 100px;
    height: 50px;
  }
  .upload_btn {
    width: 200px;
    height: 50px;
  }
  .reboot_btn {
    background-color: yellow;
    width: 100px;
    height: 50px;
    margin: 0 auto;
  }

  #svgContainer {
      position: absolute;
      width: 50px;
      height: 50px;
  }
  svg {
      width: 100%;
      height: 100%;
  }
</style>
</head>
<body>
<div id="svgContainer">
<svg fill=gold stroke=gold stroke-width="15" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 496 496" xml:space="preserve" width="50px" height="50px">
<g id="SVGRepo_bgCarrier" stroke-width="0"></g>
<g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g>
<g id="SVGRepo_iconCarrier"><g><g><g><path d="M413.968,257.096C402.968,192.184,346.464,144,280,144c-12.056,0-23.776,1.64-34.992,4.624 C225.504,120.664,194.272,104,160,104c-57.344,0-104,46.656-104,104c0,23.72,7.968,46.264,22.576,64.6 C34.48,277.336,0,314.672,0,360c0,48.52,39.48,88,88,88h312c52.936,0,96-43.064,96-96 C496,304.424,460.464,263.928,413.968,257.096z M72,208c0-48.52,39.48-88,88-88c27.4,0,52.568,12.536,69.192,33.896 c-37.368,15.056-66.944,46.28-79.136,86.376C148.008,240.088,145.984,240,144,240c-20.12,0-39.104,8.624-52.584,23.08 C78.848,247.496,72,228.248,72,208z M400,432H88c-39.704,0-72-32.304-72-72s32.296-72,71.688-72.008l5.536,0.04l2.304-3.992 C105.536,266.744,124.12,256,144,256c3.352,0,6.856,0.336,10.424,1.008l7.424,1.4l1.824-7.336C176.96,197.448,224.8,160,280,160 c60.512,0,111.672,45.28,119.008,105.32l0.56,4.592C399.84,273.24,400,276.6,400,280c0,66.168-53.832,120-120,120v16 c74.992,0,136-61.008,136-136c0-2.12-0.176-4.2-0.272-6.296C452.432,281.088,480,313.768,480,352 C480,396.112,444.112,432,400,432z">
</path><rect y="200" width="40" height="16">
</rect><rect x="250.99" y="89.01" transform="matrix(0.7071 0.7071 -0.7071 0.7071 152.9384 -151.2055)" width="16" height="40">
</rect><rect x="152" y="48" width="16" height="40">
</rect><rect x="40.98" y="100.971" transform="matrix(0.7072 0.7071 -0.7071 0.7072 94.9074 -11.2058)" width="40.002" height="16.001">
</rect><rect x="97.481" y="78.043" transform="matrix(0.3828 0.9238 -0.9238 0.3828 147.0594 -48.0366)" width="23.999" height="16">
</rect><rect x="26.082" y="149.521" transform="matrix(0.9239 0.3827 -0.3827 0.9239 63.1808 -2.5828)" width="24.001" height="16">
</rect><rect x="30.028" y="246.475" transform="matrix(-0.3828 -0.9238 0.9238 -0.3828 -186.2008 392.5536)" width="16" height="23.999">
</rect><rect x="202.512" y="74.053" transform="matrix(0.9238 0.3828 -0.3828 0.9238 48.9774 -74.0317)" width="16" height="23.999">
</rect></g></g></g></g>
</svg>
</div>
<div class="container">
<div id="FOTA">
  <h1>Firmware Over The Air</h1>
  <button type="button" onclick="history.back()" class="back_btn">Back</button><br><br><br>
  <form onsubmit="event.preventDefault(); uploadFile();">
    <div>
    <button id="rim-only-button" class="rim-only-button"></button>
    </div>
    <div id="file-input-wrapper" class="file-input-wrapper">
      <input type="file" id="firmwareFile" name="firmwareFile" class="file-input" required onchange="updateSelectedFileName();">
      <label for="firmwareFile" class="file-label">Select firmware file</label>
    </div>
    <div><span id="selectedfilename">Please select a firmware file</span></div>
    <br><br><button type="submit" class="upload_btn">Upload Firmware</button>
  </form>
  <br>
  <div id="progressText">No Firmware Loaded</div>
  <progress id="progressBar" value="0" max="100" style="width:300px;"></progress>
  <br><br>
  <button id="rebootButton" style="display: none;" onclick="reboot()" class="reboot_btn">Reboot</button>
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
  const fileInput = _('firmwareFile');
  const fileName = fileInput.files[0].name;
  _('selectedfilename').innerText = 'Ready to upload: ' + fileName;
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
  var countdownElement = _("countdown");
  countdownElement.innerHTML = timeLeft;
  var countdownTimer = setInterval(function() {
    timeLeft--;
    countdownElement.innerHTML = timeLeft;
    if (timeLeft <= 0) {
      clearInterval(countdownTimer);
      window.location.href = '/';
    }
  }, 1000);


function updateScaleFactor() {
 const vh = window.innerHeight; // Get viewport height
 const vw = window.innerWidth; // Get viewport width
 if (vh > vw) { // If in portrait mode (height > width)
  const scaleFactor = (vh + 200) / 1000; // Calculate scale factor based on height
  document.documentElement.style.setProperty('--scale-factor', scaleFactor); // Set CSS variable for scale factor
  document.body.style.overflow = 'hidden'; // Disable scrolling in portrait mode
  document.body.style.height = '100vh'; // Set body height to 100% of viewport height
  document.body.style.display = 'block'; // Use block layout
  document.body.style.justifyContent = 'unset'; // Reset horizontal alignment
//  document.body.style.alignItems = 'flex-start'; // Align items to the top
  document.querySelector('.container').classList.remove('scrollable'); // Remove scrolling class from container
  window.scrollTo(0, 0); // Scroll to the top
 } else { // If in landscape mode (width >= height)
  document.documentElement.style.setProperty('--scale-factor', 1); // Reset scale factor to 1 (no scaling)
  document.body.style.overflow = 'auto'; // Enable scrolling in landscape mode
  document.body.style.height = 'auto'; // Set body height to auto
  document.body.style.display = 'flex'; // Use flexbox layout
  document.body.style.justifyContent = 'center'; // Center content horizontally
  document.body.style.alignItems = 'center'; // Center content vertically
  document.querySelector('.container').classList.add('scrollable'); // Add scrolling class to container
 }
}
window.addEventListener('resize', updateScaleFactor); // Update scale factor on window resize
window.addEventListener('load', updateScaleFactor); // Update scale factor when page loads
}

// ----- JavaScript to move the SVG

let container = document.getElementById('svgContainer');
let position = window.innerWidth * 0.25 ;
let direction = 1;
let speed = 0.1; // Adjust the speed as needed

function moveSvg() {
  // Get the width of the window and the SVG container
  let windowWidth = window.innerWidth;
  let containerWidth = container.offsetWidth;
  // Calculate the new position
  position += direction * speed;
  // Change direction if the SVG hits the edge
  if ( position + containerWidth > ( windowWidth * 0.75 ) ) {
    position = windowWidth * 0.75 - containerWidth;
    direction *= -1;
  }
  // Change direction if the SVG hits the edge
  if ( position < ( windowWidth * 0.25 ) ) {
    position = windowWidth * 0.25;
    direction *= -1;
  }
  // Apply the new position
  container.style.left = position + 'px';
  // Request the next frame
  requestAnimationFrame(moveSvg);
}
// Start the animation
moveSvg();

</script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------