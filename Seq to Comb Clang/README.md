# S2C
/* Mehmet KÖSE */ - /* FSE */ - /* TA */
Bu kodlar ubuntu isletim sistemi uzerinde yazilmis ve ubuntu uzerinde calistirilmistir. 
main3 ve main4 fifo olan verilog dosyasinin fifosuz şekilde ciktisini üretmektedir.
main3 ; fifoların girislerini input, cikislarini output, main4 ise fifoların girişlerini output, cikislarini input olarak yazmaktadır.

dosya çalıştırılırken linux ortamında sırayla
gcc main3.c yada gcc main4.c 
./a.out  kaynakdosya.v(kaynakdosya.txt)  hedefdosyaismi.txt olarak iki argüman ile birlikte başlatılmalıdır.
kaynak dosya Dff içeren verilog kodlarını barindirmalidir, hedef dosya ismi ise ciktimizin yazilacagi dosya adidir.

//*************//
program ilk olarak 3 adet dosya açmaktadır.
source Dff içeren verilog dosyası
!!!--source dosyasını programa göndermeden önce Ana modulümüzde kullandığımız Dff modulünün tanım bloğu silinmelidir.-- !! önneml,..
temp2sc geçici olarak verileri saklamak için kullandığımız dosya
s2c pointerının gösterdiği ise çıktıların alınacağı dosyadır.
ilk olarak source dosyası satır satır okunarak veri işlenmektedir.
satır başında dff olan satırlar hariç tüm satırlar temps2c dosyasına yazılmaktadır.
dff içeren satırlar fifo structı ile linkedlist girişleri ve çıkış isimleri ayrı olarak tutulmaktadır.
main içindeki 2. while bloğunda ise program temps2c dosyasından okuduğu verileri s2c dosyasına yazarken 
satırları module, input ,output wire değişkenlerine göre düzenlemektedir.
bunları yaparken module satırında satır başı "module" değişkenine eşitse paranteze kadar yazdıktan sonra
linkedlist yapısının ilk node'ndan başlayarak tüm inputReg, outputReg verilenini parantez açıldıktan sonra yazmaktadır.
linkedlist yapısı sonlandığında temps2c dosyasında "(" 'den sonrasını yazmakta ve module satırı tamamlanmaktadır.
Bu işlem satır başı input ve output olan satırlarda da benzer şekilde yapılmaktadır. moduleDef, inputDef, outputDef fonksiyonları bu işlemler için yazılmıştır.
wire satırında ise register girişleri ve çıkışları dışarıdan alacağımız için wire satırında tekrar edenleri içerdiği için hata ile karşılaşılmıştır.
bunu çözmek için satır başın "wire" olan satıra gelindiğinde, satır kelime kelime bölünüp linkedlist içerisinde inputReg&outputReg ile aynı olanları bu satıra yazmayıp kalan kelimeleri(yani wire değişkenlerini) temps2c dosyasından alıp hedef(s2c) dosyamıza yazmaktadır. wireDef fonksiyonu bu işlem için yazılmıştır.
wireDef içerisinde ilk for ilk değişkene kadar ilerleyip durmakta ikinci for döngüsü ise değişkenleri "," yada ";" den faydalanarak ayırara linkedlistte kontrol etmektedir. virgül ve noktalıvirgüle ile ayrılan değişkenleri seekAndWrite fonksiyonuna gönderip burada linkedlist içinde gezerek aynı ada sahip input/output olup olmadığına bakılmaktadır. Fonksiyon aynı değişken daha önce inp/output olarak tanımlanmışsa    0'dan farklı bir değer döndürmektedir.
wire satırı bittikten sonra son satır olan " endmodule " satırı ile birlikte temps2c dosyamızdan tüm satırlar okunup hedef(s2c) dosyamıza yazılır. 

