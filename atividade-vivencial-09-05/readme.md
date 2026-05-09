# Atividade Vivencial 1 – Processamento Gráfico
**Alunos:** Igor Vianna Paz e Jefferson Marcanth

## Descrição da Tarefa
O objetivo deste exercício é revisar a criação de *buffers* de geometria e o envio de informações para os *shaders*. O programa atende aos seguintes requisitos:
- Ao clicar na tela com o botão esquerdo, é registrado 1 vértice.
- A cada 3 vértices capturados na tela, um triângulo é instanciado.
- Cada triângulo instanciado recebe uma cor aleatória gerada via sistema RGB.
- O mapeamento utiliza projeção paralela ortográfica (800x600), com a janela do mundo usando as mesmas dimensões da tela.
- Os cliques são tratados utilizando a função `glfwSetMouseButtonCallback` (GLFW Input Guide).

## Como compilar e executar (MinGW/MSYS2)
Caso tenha os pacotes `g++`, `glfw`, `glew` e `glm` instalados, compile no terminal rodando:

```bash
g++ main.cpp -o atividade -lglfw3 -lglew32 -lopengl32 -lgdi32
./atividade