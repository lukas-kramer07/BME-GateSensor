const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Garage Status</title>

    <style>
        :root{
      --background-color: #48515ab3;
      --darker-background-color: #243848;
      --accent-color: #18a8ad;
      --text-color:#FFFFFF;

        }
        html{
            scroll-behavior: smooth;
        }
        body {
            margin:0;font-family:Arial;
            background-image: url();
            background-position: center;
            background-repeat: no-repeat;
            background-attachment: fixed;
            background-size: cover;
            text-align: center;
            align-items: center;
        }
        .content{

            align-items: center;
            text-align: center;
            justify-content: center;
            background-color: var(--background-color);
            height: min-content;
            width: min(75%%, 80vh);
            position: relative;
            border-radius: min(4vw, 5vh);
            border: 2px solid white;
            margin: 0 auto;
            margin-top: min(5%%, 5vh);
            padding: 20px;
            color: var(--text-color);
            font-size:  min(4vw, 5vh);
        }
        .contentAccent{
            align-items: center;
            text-align: center;
            justify-content: center;
            background-color: var(--darker-background-color);
            height: min-content;
            width: 75%%;
            position: relative;
            border-radius: min(4vw, 5vh);
            border: 2px solid white;
            margin: 0 auto;
            padding: 20px;
            color: var(--text-color);
            font-size: min(4vw, 5vh);
            margin-bottom: 1vh;
        }


    </style>

</head>
<body onload="Timer()">
    <div id="StatusDiv" class="content">
        <h1 id="StatusText">Status</h1>
        <div id="Status" class="contentAccent">%Status%</div>
    </div>
    <br>
    <div id="TimerDiv" class="content">
        <h1 id="TimerDescription">Time open</h1>
        <div id="Timer" class="contentAccent">%Timer%</div>
    </div>
</body>

<script>
    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Status").innerHTML = this.responseText;
            status = this.responseText;
            if(status == "open"){
                status = "opened";
            }
            document.getElementById("TimerDescription").innerHTML = `Time ${status}`;           
    }
    };
    xhttp.open("GET", "/Status", true);
    xhttp.send();
    }, 100) ;


    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("Timer").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "/Timer", true);
    xhttp.send();
    }, 50);

</script>

</html>

)rawliteral";