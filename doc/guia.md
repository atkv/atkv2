# @kv: Guia para desenvolvimento 

## Padrão de nomes


- Macros e constantes em caixa alta: `MAX_BUFFER_SIZE`, `TRACKING_ID_PREFIX`.
- Nomes de structs e typedef's em *CamelCase*: `GtkWidget`, `TrackingOrder`.
- Funções que operam em structs em *underscore_case*: `gtk_widget_show()`, `tracking_order_process()`.
- Pointers: nothing fancy here: `GtkWidget *foo`, `TrackingOrder *bar`.
- Global variables: just don't use global variables. They are evil.
- Functions that are there, but shouldn't be called directly, or have obscure uses, or whatever: one or more underscores at the beginning: `_refrobnicate_data_tables()`, `_destroy_cache()`.
- Usar tipos específicos: `uint8_t`, `int64_t`, `double`...
- Structs com dados alinhados: ordem decrescente em tamanho do alinhamento
- Prefixo `at_` para funções;
- Prefixo `At` para structs e enums;
- Prefixo `AT_` para macros e constantes (exceto C11 Generics - use `at_`)
- Documentar o código usando estilo Doxygen

**Um exemplo**
```c
/**
 * @brief classe de exemplo
 */
typedef struct AtExemplo{
  char*   nome;
  int16_t teste;
  uint8_t pode_fazer;
}

/**
 * @brief cria um novo exemplo
 * @return instância do AtExemplo alocado
 */
AtExemplo*
at_exemplo_new();
```

## Uso do Github

O Github é uma plataforma para disponibilização do código online com várias ferramentas:

- Navegação pelo código
- Acesso do estado do código em uma branch específica
- issues (problemas, descrição de features, bugs...)
- milestones (definir metas)
- wiki
- analytics

Abaixo está um guia de estilos

1. Criar uma issue (se já não estiver) indicando a feature a ser desenvolvida. O Github devolverá o ID da issue.
2. Cada feature branch terá o nome `f###-nome` (ex: `f001-array`), onde `###` é o ID da issue associada à feature, `nome`
3. Seguir o Gitflow workflow: `master`, `develop`, `release` e `feature` branches
4. Criar um pull request para o feature branch