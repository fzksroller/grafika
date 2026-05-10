# grafika
<center>
<h1>2026_2_szamitogepi_grafika beadando <br> Impossible Labyrinth</h1>
<hr>
<h2>A beadandó célja</h2>
<hr>
<p>- A 20x20-as pályán eljutni egy golyóval a sötétkék színnel jelölt mezőre (cél), azonban három másodpercenként cserélődik a padló véletlenszerűen.</p>
<p>- A padlón lévő piros mezők visszaléptetik a golyót a kezdőpontra.</p>
<h3>A programmal a számítógépi grafika alábbi részeit szeretném szemléltetni, felhasználni:</h3>
<ul>
  <li>kamerakezelés</li>
  <li>objektumok implementálása</li>
  <li>textúrák implementálása</li>
  <li>átlátszóság</li>
  <li>árnyékolás</li>
  <li>ütközésvizsgálat</li>
</ul>
<hr>
<h2>Makefile tudnivalók</h2>
<hr>
<p>
  <table>
    <tr>
      <td>make clean</td>
      <td>meglévő futtatható, futtatáshoz szükséges fájlok törlése</td>
    </tr>
    <tr>
      <td>make all</td>
      <td>a forráskódok fordíása</td>
    </tr>
    <tr>
      <td>make run</td>
      <td>a lefordított program futtatása</td>
    </tr>
  </table>
</p>
<hr>
<h2>Repository felépíése</h2>
<hr>
<p>
feladat/<br>
├── src/<br>
│&nbsp&nbsp&nbsp&nbsp├── main.c&nbsp&nbsp&nbsp&nbsp(csak a main függvény, a játék logika)<br>
│&nbsp&nbsp&nbsp&nbsp├── texture.c&nbsp&nbsp&nbsp&nbsp(textúra függvények)<br>
│&nbsp&nbsp&nbsp&nbsp├── model_loader.c&nbsp&nbsp&nbsp&nbsp(OBJ betöltés)<br>
│&nbsp&nbsp&nbsp&nbsp├── collision.c&nbsp&nbsp&nbsp&nbsp(ütközésvizsgálat)<br>
│&nbsp&nbsp&nbsp&nbsp├── rain.c&nbsp&nbsp&nbsp&nbsp<br>
│&nbsp&nbsp&nbsp&nbsp├── math3d.c&nbsp&nbsp&nbsp&nbsp<br>
│&nbsp&nbsp&nbsp&nbsp├── app.c&nbsp&nbsp&nbsp&nbsp<br>
│&nbsp&nbsp&nbsp&nbsp└── shadow_simple.c&nbsp&nbsp&nbsp&nbsp(árnyék függvények)<br>
├── include/<br>
│&nbsp&nbsp&nbsp&nbsp├── vec3.h<br>
│&nbsp&nbsp&nbsp&nbsp├── app.h<br>
│&nbsp&nbsp&nbsp&nbsp├── math3d.h<br>
│&nbsp&nbsp&nbsp&nbsp├── rain.h<br>
│&nbsp&nbsp&nbsp&nbsp├── model.h<br>
│&nbsp&nbsp&nbsp&nbsp├── texture.h<br>
│&nbsp&nbsp&nbsp&nbsp├── model_loader.h<br>
│&nbsp&nbsp&nbsp&nbsp├── collision.h<br>
│&nbsp&nbsp&nbsp&nbsp├── shadow_simple.h<br>
│&nbsp&nbsp&nbsp&nbsp└── game.h<br>
├── assets/<br>
│&nbsp&nbsp&nbsp&nbsp├── help.png<br>
│&nbsp&nbsp&nbsp&nbsp├── win.png<br>
│&nbsp&nbsp&nbsp&nbsp├── texture.jpg<br>
│&nbsp&nbsp&nbsp&nbsp├── wood_box.obj<br>
│&nbsp&nbsp&nbsp&nbsp├── wood_box.mtl<br>
│&nbsp&nbsp&nbsp&nbsp└── wood_box_texture.png<br>
└── Makefile<br>
</p>
<hr>
<h2>Asset-ek</h2>
<hr>
<p>Az alábbi linken megtalálhatóak a programhoz használt asset-ek .zip formátumba tömörítve.<br>(A program az asset-ek nélkül is használható.)</p>
<a href="https://drive.google.com/drive/folders/1G70U9NmCNc_fNXtVzun4r4gMDLDyDk0X?usp=sharing">https://drive.google.com/file/d/1NU5UzoTfuFGUAPGJbFAqpruhD2nLVtpP/view?usp=drive_link</a>
<hr>
<h2>Irányíás</h2>
<hr>
<p>A program irányíása QWERTZ billentyűzet kiosztást használ!</p>

<table>
<tr>
  <td>Mozgás</td>
  <td>Egyéb beállítások</td>
</tr>
<tr>
  <td>W - Előremenet</td>
  <td>F1 - Súgó</td>
</tr>
  <tr>
  <td>A - Balramenet</td>
  <td>F2 - Pálya újragenerálása</td>
</tr>
  <tr>
  <td>S - Hátramenet</td>
  <td>Esc - Egér feloldása</td>
</tr>
  <tr>
  <td>D - Jobbramenet</td>
  <td>R - eső ki/be kapcsolása</td>
</tr>
</tr>
  <tr>
  <td>Space - respawn</td>
  <td>T - árnyék ki/be kapcsolása</td>
</tr>
</tr>
  <tr>
  <td>Q - Kamera elforgatás balra</td>
  <td>Egér - Kamera mozgatás</td>
</tr>
</tr>
  <tr>
  <td>E - Kamera elforgatás jobbra</td>
  <td>&nbsp</td>
</tr>
</table>
</center>



