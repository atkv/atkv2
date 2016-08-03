Biblioteca de Visão Computacional com foco no framework IFT[1]

| Branch  | Building | Coverage |
| ------------- | ------------- | ------------- |
| Develop  | ![Build Status](https://travis-ci.org/atkv/atkv2.svg?branch=develop) | ![Coverage Status](https://coveralls.io/repos/github/atkv/atkv2/badge.svg?branch=develop) |
| Master  | ![Build Status](https://travis-ci.org/atkv/atkv2.svg?branch=master) | ![Coverage Status](https://coveralls.io/repos/github/atkv/atkv2/badge.svg?branch=master) |

## Como compilar

~~~~bash
$ git clone https://github.com/atkv/atkv2.git
$ cd atkv2
$ mkdir build && cd build
$ cmake .. && make
$ sudo make install
~~~~

No Linux, ele instalará por padrão em `/usr/local/lib`, você pode alterar para `/usr/lib`(utilizando a flag `-DCMAKE_INSTALL_PREFIX`) ou [incluir](http://stackoverflow.com/questions/17889799/libraries-in-usr-local-lib-not-found) `/usr/local/lib` no ldconfig.

## Como usar

Basta criar o seu código (ex: `seu_programa.c` carrega e exibe um PNG) e compilá-lo:
~~~c
#include <at/gui.h>
int main(){
  AtArrayU8* imagem = at_arrayu8_read_png("imagem.png");
  at_display_show_image("imagem",imagem);
  at_display_wait_key();
  return 0;
}
~~~

~~~bash
$ gcc seu_programa.c -o seu_programa `pkg-config --libs --cflags atkv`
~~~

## API e Tutoriais

Você pode compilar a documentação utilizando a flag `-DBUILD_DOCS=ON` no processo de compilação, ou então acesse-o online [aqui](http://atkv.github.io/docs).

## Módulos

\@kv está separado em módulos (seu programa pode não precisar de todos os módulos):

- **CORE** (contém praticamente o AtArray e outras estruturas básicas)
- **IMGPROC** (contém o IFT, o carregamento de imagens JPG, PNG, DICOM e NIFTI)
- **GUI** (contém código para interface gráfica)

## Dependências

Para compilar tudo, só basta estes dois pacotes:

- [CMake](https://cmake.org/) 3.1+ (para compilar o código)
- [GTK+](http://www.gtk.org/) 3.18+ (para módulo **GUI**)

Se não desejar compilar o módulo **GUI** (usando `-DBUILD_GUI=OFF`), então não é necessário o GTK+, basta ter:

- [libpng](http://www.libpng.org/pub/png/libpng.html) 1.6+ (para PNG, no módulo **IMGPROC**)
- [libjpeg](http://libjpeg.sourceforge.net/) 8.0+ (para JPEG, no módulo **IMGPROC**)

Se também não desejar compilar o módulo **IMGPROC** (usando `-DBUILD_IMGPROC=OFF`), então você deseja apenas compilar o **CORE**, que precisa de:

- [ZLib](http://www.zlib.net/) 1.2+ (para os algoritmos de compressão)

Os testes são feitos em [CMocka](https://cmocka.org/) 1.0+.

## Referências

[1] A. X. A. Falcão, J. Stolfi, R. de Alencar Lotufo, A. X. Falcao, J. Stolfi, and R. de Alencar Lotufo, “The Image Foresting Transform: Theory, Algorithms, and Applications,” IEEE Trans. Pattern Anal. Mach. Intell., vol. 26, no. 1, pp. 19–29, Jan. 2004. 


