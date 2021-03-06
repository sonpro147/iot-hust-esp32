const char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
  <head>
    <title>HUST-IOT</title>
    <meta charset="utf-8">
    <style>
      .header{        
        height:30%;
      }
      #header-wrapper{
        height: 15vh;
        background-color: #18F043;
        display: flex;
        justify-content: center;
        align-items:center;
        margin-bottom: 50px;
      }
      #body-wrapper{
        display: flex;
        flex-direction: row;
        justify-content: space-around;
      }

      .column{
        border: 1px solid black;
        border-radius: 10px;
        padding: 10px 10px 10px 10px;
        background-color: #A5F4F4;
      }

      #column-3{
        display: flex;
        flex-direction: column;
        align-items: center;
      }

      .button-3{
        height: 50px;
        width: 150px;
        border-radius: 5px;
      }
      
      .button-2{
        height: 30px;
        width: 100px;
        border-radius: 5px;
        margin-top:10px;
      }

    </style>
  </head>

  <script type="text/javascript">
  
    function tuoiCay1() {
      var xhttp = new XMLHttpRequest();
      xhttp.open('GET','/tuoiCay1',true);
      xhttp.send();
    }

    function tuoiTatCa() {
      tuoiCay1();
      //tuoiCay2();
    }

    function send_doam() {
      var x1 = document.getElementById("sendtext1").value;
      if(x1 != ""){
          var xhttp = new XMLHttpRequest();
          xhttp.open('GET','/x1?namex1=' + x1,true);
          xhttp.send();
      }
    }

  function loadData() {
      var xhttp = new XMLHttpRequest();
      //nhan du kieu tu esp gui len web
      xhttp.onreadystatechange = function(){
          if(this.readyState == 4 && this.status == 200){
              var DataVDK = xhttp.responseText;
              var ahihi = JSON.parse(DataVDK);
              document.getElementById("nhietdo").innerHTML = ahihi.ND;
              document.getElementById("doamkk").innerHTML = ahihi.DAkk;
              document.getElementById("doam1").innerHTML = ahihi.DA1;
              document.getElementById("limit").innerHTML = ahihi.limit;
              
              console.log(DataVDK);
          }
      }
      xhttp.open('GET','/loadData',true);
      xhttp.send();
      setTimeout(function(){ loadData() }, 500);
  }
  </script>

  <body onload = "loadData()">
  <div id="header-wrapper">
    <h1 class="header">H??? th???ng t?????i c??y t??? ?????ng</h1>
  </div>
  <div id="body-wrapper">
    <div class="column">
      <h2>T??nh Tr???ng</h2>
      <h3>
        Nhi???t ????? kh??ng kh??: <span id="nhietdo">0</span> ????? C <br>
      </h3>
      <h3>
        ????? ???m
        <ul>
          <li>
            ????? ???m kh??ng kh??: <span id="doamkk">0</span> % <br>
          </li>
          <li>
            ????? ???m ?????t:
            <ul>
              <li>
                C??y 1: <span id="doam1">0</span> % <br>
              </li>
              <li>
                C??y 2: <span id="doam2">0</span> % <br>
              </li>
            </ul>
          </li>
          
        </ul>
      </h3>   
    </div>
    <div class="column">
      <h2>
        Ch??? ????? t?????i t??? ?????ng:
      </h2>
      
      <h3>T?????i khi ????? ???m d?????i: <span id="limit">0</span> %</h3>
      <input id="sendtext1" type="number" min="1" max="100"> %
      <div class="button-wrapper">
        <button class="button-2" type="button" onclick="send_doam()">SET</button><br><br>  
      </div>  
    </div>
    <div class="column" id="column-3">
      <h2>Ch??? ????? t?????i th??? c??ng</h2>
      <button class="button-3" type="button" onclick="tuoiTatCa()">T?????i t???t c???</button><br><br>
      <button class="button-3" type="button" onclick="tuoiCay1()">T?????i c??y 1</button><br><br>
      <button class="button-3" type="button" onclick="">T?????i c??y 2</button>    
    </div>
  </div>  
  </body>
</html>
)=====";
