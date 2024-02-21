	// =========================== set background image =================================================================
	// const canvas = document.querySelector("#canvas_x");
	// const canvas1 = document.querySelector("#canvas_x");
	// const ctx1 = canvas1.getContext("2d");
	// const ctx2 = canvas1.getContext("2d");

	// if (canvas.getContext) {
	// 	var ctx = canvas.getContext("2d");
	// 	base_image = new Image();
	// 	base_image.src = "sky.png";
	// 	base_image.onload = function () {
	// 		ctx.drawImage(base_image, 0, 0, 500, 300);
	// 		ctx1.beginPath();
	// 		ctx1.arc(100, 100, 75, 0, 2 * Math.PI, false);
	// 		ctx1.fillStyle = "white";
	// 		ctx1.fill();
	// 		ctx1.lineWidth = 5;
	// 		ctx1.strokeStyle = "#003300";
	// 		ctx1.stroke();
	// 	}
	// }

	// function rose() {
	// 	document.getElementsByTagName("comp")[0].value = ku + "\xB0";
	// 	ctx1.save();
	// 	ctx1.translate(100, 100);
	// 	ctx1.rotate((kul - 45) * Math.PI / 100);
	// 	base_imge = new Image();
	// 	base_image.src = "index.png";
	// 	base_image.src = "rose2.jpg";
	// 	ctx1.drawImage(base_image, -50, -50, 100, 100);
	// 	ctx1.restrore();
	// }

	// ctx2.beginPath();
	// ctx2.arc(100, 100, 75, 0, 2 * Math.PI, false);
	// ctx2.fillStyle = "white";
	// ctx2.fill();

	// =========================== variables =================================================================
	var speed = 100;
	var dire = 0;		  // direction
	var dist = 0;		  // distance
	var led_state = 0;    // led on/off, 0 is off, 1 is on

	// =========================== set speed =================================================================
	$('#speed').val(100);

	document.getElementById("speed").oninput = function () {
		$('#showSpeed').html(this.value);
	}

	$('#speed').click(function () {
		speed = this.value; // Update the current dist value (each time you drag the slider handle and stop)
		console.log("speed: " + speed);

		xmlHttp.open("GET", '/?speed=' + speed, true);
		xmlHttp.send(null);
	});

	// =========================== set the turning degree ====================================================
	$('#turning').val(0);

	var xmlHttp = new XMLHttpRequest();

	$(document).ready(function () {
		$("#steering_wheel").roundSlider({
			radius: 95,
			min: -180,
			max: 180,
			circleShape: "pie",
			sliderType: "min-range",
			value: 0,
			startAngle: 315,

			change: function (args) {
				dire = args.value;			 // direction variable set
				console.log("direction: " + dire);
            setCompass(dire);
				$('#turning').val((dire > 0 ? " Right " : " Left ") + Math.abs(dire) + "\xB0");
				xmlHttp.open("GET", '/?dire=' + dire, true);
				xmlHttp.send(null);
			}
		});

      setProgressBar(1);

		/*
		setInterval(function () {
			const rand = getRandom(1, 100).toString() + "%";
         $('#progress-bar').animate({width: rand}, 500);
         $('#lider_value').text("Distance: " + rand);
		}, 2000);
      
      setInterval(function () {
			const rand = getRandom(1, 360);
         $('#compass-pointer').animate({rotate: -rand+'deg'}, 500);
         $('#compass_value').text("Angle: " + rand.toString() + " deg");
		}, 3000);
      */

	});

	// =========================== set the driving distance ================================================
	$('#set_go').val(0);
	$('#moving').val(0);

	document.getElementById("set_go").oninput = function () {
		$('#showDist').html(this.value);
	}

	$('#set_go').click(function () {
		dist = this.value; // Update the current dist value (each time you drag the slider handle and stop)
		$('#moving').val((dist > 0 ? " Forward " : " Backward ") + Math.abs(dist) + "cm");
		console.log("distance: " + dist);

      setProgressBar(dist);

		xmlHttp.open("GET", '/?dist=' + dist, true);
		xmlHttp.send(null);
	});

	// =========================== set the LED state ========================================================
	function toggleCheckbox(element) {
		if (element.checked) {
			led_state = 1;
		}
		else {
			led_state = 0;
		}
		console.log("led_state: " + (led_state == 0 ? "OFF" : "ON"));

		xmlHttp.open("GET", '/?led_state=' + led_state, true);
		xmlHttp.send(null);
	}

	// =========================== print string from Mega =================================================================

	setInterval(function ()									// timer inreval function
	{

		var xhttp = new XMLHttpRequest();

		xhttp.onreadystatechange = function () {
			if (this.readyState == 4 && this.status == 200) {
				let resp = this.responseText;				//  input string from server c++/Mega
				$('#mega_string').html(resp);

				let pos1 = resp.indexOf("=");
				let command = resp.substring(0, pos1);
				let val = parseInt(resp.substring(pos1 + 1));

				if (command == "Lid") {
               setProgressBar(val);
				}
				else if (command == "Com") {
               setCompass(val);
				}
			}
		};

		xhttp.open("GET", "/from_MEGA", true);							// request string from server
		xhttp.send();
	}, 500);
   

	function getRandom(min, max) {
		return Math.floor(Math.random() * (max - min + 1) ) + min;
	}

   function setCompass(angle) {
      $('#compass_value').text("Angle: " + angle.toString() + " deg");
      $('#compass-pointer').animate({rotate: -angle+'deg'}, 500);
   }

   function setProgressBar(dist) {
      $('#lider_value').html("Distance: " + dist + "cm");
      //$("#progress-bar").css("width", dist.toString() + "%");
      $('#progress-bar').animate({width: dist.toString() + "%"}, 500);
   }