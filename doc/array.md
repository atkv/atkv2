# Usando Array

Este tutorial serve para ensinar a usar as funções de array. O Array é uma estrutura para vetores de múltiplas dimensões, parecido com [ndarray](http://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.html) do [numpy](http://www.numpy.org/) e [Mat](http://docs.opencv.org/trunk/d3/d63/classcv_1_1Mat.html#gsc.tab=0) do [OpenCV](http://docs.opencv.org/trunk). Para criar um novo array, existem algumas versões:

- `at_arrayu8_new(uint8_t dim, uint64_t *shape)`: cria um novo array alocando memória com `dim` dimensões e no formato `shape`
- `at_arrayu8_new_with_data(uint8_t dim, uint64_t *shape, uint8_t* data, uint8_t copy)`: utiliza um vetor de dados já existente, alocando uma cópia (se `copy == true`) ou a referenciando.
- `at_arrayu8_zeros(uint8_t dim, uint64_t* shape)`: `at_arrayu8_new` + atribuindo 0 (zero) aos elementos.
- `at_arrayu8_ones(uint8_t dim, uint64_t* shape)`: `at_arrayu8_new` + atribuindo 1 (um) aos elementos.

Cada elemento será um inteiro sem sinal de 8 bits. Se desejar, troque o sufixo `u8` por `u16`, `u32` e `u64` para inteiros sem sinal, ou `i8`, `i16`, `i32` e `i64` para inteiros com sinal, ou `f32` e `d64` para float e double.

~~~c
// ex_new_array.c
int main(){
  uint64_t shape[4] = {2,4,3,5};
  AtArrayU8* array = at_arrayu8_new(3, shape);
  printf("%d elements\n", array->h.nelem);
  at_arrayu8_destroy(&array);
  return 0;
}
~~~

Compilando:


~~~bash
$ gcc ex_new_array.c -o ex_new_array -lz -latkv_core
$./ex_new_array
1200 elements
~~~