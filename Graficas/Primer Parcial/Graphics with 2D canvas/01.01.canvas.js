function init(){
    setInterval(updateCanvas, 700);
    updateCanvas();
}

function updateCanvas(){
  /*   var width = window.innerWidth;
    var height = 100;

    var myCanvas = document.getElementById("myCanvas");
        myCanvas.width = width;
        myCanvas.height = height;
    
    var context = myCanvas.getContext("2d");
    context.clearRect(0, 0, width, height);
    
    context.fillStyle = "#FCEAB8";
    context.fillRect(0, 0, width, height);

    var circleSize = 10;
    var gaps = circleSize + 10;
    var widthCount = parseInt(width / gaps);
    var heightCount = parseInt(height / gaps);
    var aColors = ["#43A9D1", "#EFA63B", "#EF7625", "#5E4130"];
    var aColorsLength = aColors.length;

    for (var x = 0; x < widthCount; x++) {

        for (var y = 0; y < heightCount; y++) {

            context.fillStyle = aColors[parseInt(Math.random() * aColorsLength)];
            context.beginPath();
            context.arc(circleSize + gaps * x, circleSize + gaps * y, circleSize, 0, Math.PI * 2, true);
            context.closePath();
            context.fill();
            
        }
    }
    var cnvPalau = document.getElementById("palau");
    var wid = cnvPalau.width;
    var hei = cnvPalau.height;
    
    var context = cnvPalau.getContext("2d");
    context.fillStyle = "#4AADD6";
    context.fillRect(0, 0, wid, hei);
    
    context.fillStyle = "#FFDE00";
    context.arc(wid / 2.3, hei / 2, 40, 0, 2 * Math.PI, false);
    context.fill();
    */

    /* var canvas = document.getElementById("greece");
    var wid = canvas.width;
    var hei = canvas.height;

    var lineHeight = hei / 9;

    var context = canvas.getContext("2d");
    context.fillStyle = "#000080";
    context.fillRect(0, 0, wid, hei);

    context.lineWidth = lineHeight;
    context.strokeStyle = "#ffffff";

    var offset = lineHeight / 2;

    for (var i = 1; i < 8; i += 2) {

        context.moveTo(0, i * lineHeight + offset);
        context.lineTo(wid, i * lineHeight + offset);

    }

    var offset = lineHeight / 2;

    for (var i = 1; i < 8; i += 2) {
        context.moveTo(0, i * lineHeight + offset);
        context.lineTo(wid, i * lineHeight + offset);

    }
    context.stroke();

    context.fillRect(0, 0, lineHeight * 5, lineHeight * 5);
    context.beginPath();
    context.moveTo(0, lineHeight * 2.5);
    context.lineTo(lineHeight * 5, lineHeight * 2.5);
    context.moveTo(lineHeight * 2.5, 0);
    context.lineTo(lineHeight * 2.5, lineHeight * 5 + 1);
    context.stroke(); */

   /*  var canvas = document.getElementById("guyana");
    var wid = canvas.width;
    var hei = canvas.height;
    var context = canvas.getContext("2d");
    context.fillStyle = "#009E49";
    context.fillRect(0, 0, wid, hei);

    fillTriangle(context, 0, 0, wid, hei / 2, 0, hei, "#ffffff");
    fillTriangle(context, 0, 10, wid - 25, hei / 2, 0, hei - 10, "#FCD116");
    fillTriangle(context, 0, 0, wid / 2, hei / 2, 0, hei, "#000000");
    fillTriangle(context, 0, 10, wid / 2 - 16, hei / 2, 0, hei - 10, "#CE1126");
 */
    var canvas = document.getElementById("israel");
    var wid = canvas.width;
    var hei = canvas.height;
    var context = canvas.getContext("2d");

    var radian = Math.PI*180;    

    var tilt = radian * 180;

    var baseX = wid / 2;
    var baseY = hei / 2;

    var radius = 24;

    var stripHeight = 14;

    context.lineWidth = 5;

    createTrinagle(context,
        baseX + Math.sin(0) * radius,
        baseY + Math.cos(0) * radius,
        baseX + Math.sin(radian * 120) * radius,
        baseY + Math.cos(radian * 120) * radius,
        baseX + Math.sin(radian * 240) * radius,
        baseY + Math.cos(radian * 240) * radius,
        null, "#0040C0");

    createTrinagle(context,
        baseX + Math.sin(tilt) * radius,
        baseY + Math.cos(tilt) * radius,
        baseX + Math.sin(radian * 120 + tilt) * radius,
        baseY + Math.cos(radian * 120 + tilt) * radius,
        baseX + Math.sin(radian * 240 + tilt) * radius,
        baseY + Math.cos(radian * 240 + tilt) * radius,
        null, "#0040C0");

    context.lineWidth = stripHeight;
    context.beginPath();
    context.moveTo(0, stripHeight);
    context.lineTo(wid, stripHeight);
    context.moveTo(0, hei - stripHeight);
    context.lineTo(wid, hei - stripHeight);
    context.closePath();
    context.stroke();

};

/* function fillTriangle(context, x1, y1, x2, y2, x3, y3, color) {
    context.fillStyle = color;
    context.beginPath();
    context.moveTo(x1, y1);
    context.lineTo(x2, y2);
    context.lineTo(x3, y3);
    context.lineTo(x1, y1);
    context.closePath();
    context.fill();
}  */

function createTriangle(context, x1, y1, x2, y2, x3, y3, fillColor, strokeColor) {
    context.beginPath();
    context.moveTo(x1, y1);
    context.lineTo(x2, y2);
    context.lineTo(x3, y3);
    context.lineTo(x1, y1);
    context.closePath();
    if (fillColor) {
        context.fillStyle = fillColor;
        context.fill();
    }
    if (strokeColor) {
        context.strokeStyle = strokeColor;
        context.stroke();

    }
}