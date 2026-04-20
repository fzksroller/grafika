# grafika
<center>
<h1>2026_2_szamitogepi_grafika gyakorlati feladatok / beadando</h1>
<hr>
<h2>A beadandó célja</h2>
<hr>
<p>Egy TPS (third-person view) nézetből irányítható golyót eljuttatni a pája ¨kezdő pontjától¨ a célig.</p>
<h3>A programmal a számítógépi grafika alábbi részeit szeretném szemléltetni, felhasználni:</h3>
<ul>
  <li>kamerakezelés</li>
  <li>objektumok</li>
  <li>textúrák</li>
  <li>fények</li>
  <li>átlátszóság</li>
  <li>árnyék</li>
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
│&nbsp&nbsp&nbsp&nbsp└── shadow.c&nbsp&nbsp&nbsp&nbsp(árnyék függvények)<br>
├── include/<br>
│&nbsp&nbsp&nbsp&nbsp├── vec3.h<br>
│&nbsp&nbsp&nbsp&nbsp├── model.h<br>
│&nbsp&nbsp&nbsp&nbsp├── texture.h<br>
│&nbsp&nbsp&nbsp&nbsp├── model_loader.h<br>
│&nbsp&nbsp&nbsp&nbsp├── collision.h<br>
│&nbsp&nbsp&nbsp&nbsp├── shadow.h<br>
│&nbsp&nbsp&nbsp&nbsp└── game.h<br>
├── assets/<br>
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
<a href="https://drive.google.com/file/d/1NU5UzoTfuFGUAPGJbFAqpruhD2nLVtpP/view?usp=drive_link">https://drive.google.com/file/d/1NU5UzoTfuFGUAPGJbFAqpruhD2nLVtpP/view?usp=drive_link</a>
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
  <td>Q - Kamera elforgatás balra</td>
</tr>
  <tr>
  <td>A - Balramenet</td>
  <td>E - Kamera elforgatás jobbra</td>
</tr>
  <tr>
  <td>S - Hátramenet</td>
  <td>F1 - Súgó</td>
</tr>
  <tr>
  <td>D - Jobbramenet</td>
  <td>Esc - Egér feloldása</td>
</tr>
</table>
</center>



