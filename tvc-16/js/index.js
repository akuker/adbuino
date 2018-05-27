var stage = document.getElementById('stage'),
  ctx = stage.getContext('2d'),
  particles = [];

var TO_RADIANS = Math.PI / 180;

var colors = ['#32cd32', '#7d4071', '#f1c711'];
var accent_color = colors[Math.floor(Math.random() * colors.length)];

//grab uptimes from the textfile
$.get("uptime", function (result) {
  $("#uptime").html(result);
});

$.get("uptimedetail", function (result) {

  $(".footer-container").attr("title", "This COOL SYSTEM has been running " + result);
});

//SVG elements
var imgArray = new Array();

imgArray[0] = new Image();
imgArray[0].src = './img/symbol.svg';

imgArray[1] = new Image();
imgArray[1].src = './img/symbol2.svg';

imgArray[2] = new Image();
imgArray[2].src = './img/symbol3.svg';

imgArray[3] = new Image();
imgArray[3].src = './img/symbol4.svg';

imgArray[4] = new Image();
imgArray[4].src = './img/symbol5.svg';

imgArray[5] = new Image();
imgArray[5].src = './img/symbol6.svg';

imgArray[6] = new Image();
imgArray[6].src = './img/symbol7.svg';

imgArray[7] = new Image();
imgArray[7].src = './img/symbol8.svg';

imgArray[8] = new Image();
imgArray[8].src = './img/symbol9.svg';

imgArray[9] = new Image();
imgArray[9].src = './img/symbol10.svg';

imgArray[10] = new Image();
imgArray[10].src = './img/symbol11.svg';

imgArray[11] = new Image();
imgArray[11].src = './img/symbol12.svg';

imgArray[12] = new Image();
imgArray[12].src = './img/symbol13.svg';

imgArray[13] = new Image();
imgArray[13].src = './img/cart2.svg';

var radius = 2, // how large to draw the particle
  speed = 0.1, // how fast the particle should move
  variance = 0.5, // the maximum number of variance that can occur in directions along an axis (1 = 20 possible amounts)
  count = 70; // how many particles to generate

// set the canvas to the size of the window
stage.width = window.innerWidth;
stage.height = window.innerHeight;

function instatiateParticles() {
  particles = [];

  //Compute number of particles to generate depending on the screen width
  count = stage.width * 50 / 1920;

  for (var i = 0; i < count; i++) {
    // create a new particle
    var p = {};

    //assign it an image
    p.svg = imgArray[Math.floor(Math.random() * imgArray.length)];

    //random rotation
    p.rotation = Math.random() * 360;

    // place the particle in the center, offset by somewhere between -scatter and scatter
    p.x = Math.random() * (stage.width);
    p.y = Math.random() * (stage.height);

    // set the particle moving at a random direction. Number will always have one decimal place, at most
    p.xVel = speed * parseFloat((Math.random() * (variance * 2) - variance).toFixed(1));
    p.yVel = speed * parseFloat((Math.random() * (variance * 2) - variance).toFixed(1));

    // add the particle to an array
    particles.push(p);
  }
}

function drawRotatedImage(image, x, y, w, h, angle) {
  // save the current co-ordinate system 
  // before we screw with it
  ctx.save();

  // move to the middle of where we want to draw our image
  ctx.translate(x, y);

  // rotate around that point, converting our 
  // angle from degrees to radians 
  ctx.rotate(angle * TO_RADIANS);

  // draw it up and to the left by half the width
  // and height of the image 
  ctx.drawImage(image, -(image.width / 2), -(image.height / 2), w, h);

  // and restore the co-ords to how they were when we began
  ctx.restore();
}

// this function runs once a frame
function render() {
  // clear the canvas
  ctx.clearRect(0, 0, stage.width, stage.height);

  // calculate the new positions of, then draw, each particle
  for (var i = 0; i < particles.length; i++) {
    var p = particles[i];

    // move the particle along its chosen direction
    p.x += p.xVel;
    p.y += p.yVel;

    // reverse the x or y direction of the particle if it hits the wals
    if (p.x + (radius / 2) > stage.width) p.xVel = Math.abs(p.xVel) * -1;
    if (p.x - (radius / 2) < 0) p.xVel = Math.abs(p.xVel);
    if (p.y + (radius / 2) > stage.height) p.yVel = Math.abs(p.yVel) * -1;
    if (p.y - (radius / 2) < 0) p.yVel = Math.abs(p.yVel);

    // draw the particle
    //ctx.drawImage(p.svg, p.x, p.y, 100, 100);
    drawRotatedImage(p.svg, p.x, p.y, 100, 100, p.rotation);
  }
}

function addEvent(object, type, callback) {
  if (object == null || typeof (object) == 'undefined') return;
  if (object.addEventListener) {
    object.addEventListener(type, callback, false);
  } else if (object.attachEvent) {
    object.attachEvent("on" + type, callback);
  } else {
    object["on" + type] = callback;
  }
};

// http://www.paulirish.com/2011/requestanimationframe-for-smart-animating/

instatiateParticles();

window.requestAnimFrame = (function () {
  return window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    function (callback) {
      window.setTimeout(callback, 1000 / 60);
    };
})();

addEvent(window, "resize", function (event) {
  stage.width = window.innerWidth;
  stage.height = window.innerHeight;
  instatiateParticles();
});


(function animloop() {
  requestAnimFrame(animloop);
  render();
})();