# @kv

Biblioteca de Visão Computacional com foco no framework IFT[1]

| Branch  | Status |
| ------------- | ------------- |
| Develop  | [![Build Status](https://travis-ci.org/atkv/atkv_cuda.svg?branch=develop)](https://travis-ci.org/atkv/atkv_cuda) [![Coverage Status](https://coveralls.io/repos/github/atkv/atkv_cuda/badge.svg?branch=develop)](https://coveralls.io/github/atkv/atkv_cuda?branch=develop)
|
| Master  | [![Build Status](https://travis-ci.org/atkv/atkv_cuda.svg?branch=master)](https://travis-ci.org/atkv/atkv_cuda) [![Coverage Status](https://coveralls.io/repos/github/atkv/atkv_cuda/badge.svg?branch=master)](https://coveralls.io/github/atkv/atkv_cuda?branch=master)
 |

## Como compilar

~~~~bash
$ git clone https://github.com/atkv/atkv_cuda.git
$ cd atkv_cuda
$ mkdir build && cd build
$ cmake .. && make
$ sudo make install
~~~~

No Linux, ele instalará por padrão em `/usr/local/lib`, você pode alterar para `/usr/lib`(utilizando a flag `-DCMAKE_INSTALL_PREFIX`) ou [incluir](http://stackoverflow.com/questions/17889799/libraries-in-usr-local-lib-not-found) `/usr/local/lib` no ldconfig.

## Como usar

~~~bash
$ gcc seu_programa.c -o seu_programa -latkv_core -latkv_imgproc -lz -ljpeg -lpng -I<caminho_dos_cabecalhos> -L<caminho_das_bibliotecas>
~~~

Se você instalou em um lugar já rastreado por ldconfig, então o comando se reduz a:

~~~bash
$ gcc seu_programa.c -o seu_programa -latkv_core -latkv_imgproc -lz -ljpeg -lpng -I<caminho_dos_cabecalhos> 
~~~

Quando o nosso arquivo PkgConfig estiver pronto (ainda não está), isto será reduzido a:

~~~bash
$ gcc seu_programa.c -o seu_programa `pkg-config --libs --cflags atkv`
~~~

## API e Tutoriais

Você pode compilar a documentação utilizando a flag `-DBUILD_DOCS=ON` no processo de compilação, ou então acesse-o online [aqui](http://atkv.github.io/docs).

## Módulos

@kv está separado em módulos (seu programa pode não precisar de todos os módulos):

- Core (contém praticamente o AtArray e outras estruturas básicas)
- Imgproc (contém o IFT, o carregamento de imagens JPG, PNG, DICOM e NIFTI)

## Dependências

- cmake (para compilar o código)
- zlib (para os algoritmos de compressão)
- libpng
- libjpeg

## Referências

[1] A. X. A. Falcão, J. Stolfi, R. de Alencar Lotufo, A. X. Falcao, J. Stolfi, and R. de Alencar Lotufo, “The Image Foresting Transform: Theory, Algorithms, and Applications,” IEEE Trans. Pattern Anal. Mach. Intell., vol. 26, no. 1, pp. 19–29, Jan. 2004. 


