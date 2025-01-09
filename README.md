# TF_FCG
Marcelo Gross Borges - 00344298

## Contribuições de Marcelo:
- Importação e busca por objetos/modelos adequados para o projeto
- Mapeamento de texturas em todos os objetos
- Modelos de Iluminação Difusa e Blinn-Phong
- Modelos de Interpolação de Phong e Gouraud
- Câmera livre e look-at
- Implementação dos movimentos do submarino
- Movimentação com curva Bézier de grau 3
- Três tipos de testes de intersecção (colisões ponto-plano, cubo-plano e cubo-cubo)

## Uso de ChatGPT
  O ChatGPT foi utilizado durante a realização desse trabalho principalmente para realizar refatoração, limpeza e comentários no código, além de fazer esse README. Para implementação das features e requisitos principais IAs em geral não foram de muita ajuda.

## Desenvolvimento e aplicação de conceitos
  - **Câmeras**: A câmera do tipo LookAt foi implementada como modo default de câmera, e está sempre posicionada atrás e levemente acima do submarino, se movimentando e rotacionando em consjunto com o submarino. A câmera do tipo FreeCamera foi implementada de forma a deixar o submarino estático e permitir uma movimentação totalmente livre por parte do usuário, controlada por mouse e teclado. 
    
  - **Colisões**: Todos os objetos possuem colisões com o submarino. As colisões foram implementadas a partir de uma classe Object, que serve para mapear uma bounding box nos objetos. Foram implementadas três tipos de colisões:

    - Colisão Ponto-Plano (PointPlaneCollision): Detecta a colisão entre um segmento de linha (definido por dois pontos) e um plano. É utilizada como base para   detecção de colisões mais complexas e verifica se o ponto de intersecção está dentro dos limites do plano definidos por sua largura, altura e comprimento.
    - Colisão Cubo-Plano (CubePlaneCollision): Verifica a colisão entre um cubo em movimento e um plano estático. O algoritmo trabalha testando as arestas do cubo contra o plano em cada direção (x, y, z) separadamente, utilizando a colisão ponto-plano como base. É útil para detectar colisões com paredes, chão e outros objetos planares do cenário.
    - Colisão Cubo-Cubo (CubeCubeCollision): Implementa a detecção de colisão entre dois cubos (bounding boxes) em movimento. Utiliza uma abordagem de decomposição por eixos, verificando separadamente as colisões em cada direção (x, y, z) através da função auxiliar PointCubeCollision. É essencial para detectar colisões entre o submarino e outros objetos tridimensionais do cenário.
 
 
  - **Iluminação**:
    - Modelo de Iluminação de Lambert:
      Modelo de iluminação difusa que considera apenas o ângulo entre a normal da superfície e a direção da luz, resultando em uma aparência fosca. Aplicado para computar a iluminação do submarino
    - Modelo de Iluminação de Blinn-Phong:
      Extensão do modelo Phong que adiciona reflexão especular usando o vetor halfway entre a visão e a luz, criando reflexos e brilhos nas superfícies. Aplicado para computar a iluminação do tubarão e do peixe

    - Modelo de Interpolação de Gouraud:
      Interpola as normais dos vértices e calcula a iluminação para cada fragmento, produzindo resultados mais suaves e realistas, mas com maior custo computacional. Foi utilizado na iluminação do submarino. 
    - Modelo de Interpolação de Phong
      Interpola as normais dos vértices e calcula a iluminação para cada fragmento, produzindo resultados mais suaves e realistas, mas com maior custo computacional. Foi utilizado na iluminação do tubarão e do peixe.
    
    
  - **Bézier**: Foi implementada uma curva de bézier de grau 3 para fazer o tubarão nadar em círculos.

## Manual
### Objetivos:
- Coletar os peixes enquanto foge do tubarão

### Mapeamento de teclas:
- A, D == Rotacina o submarino
- W, S == Acelera e freia o submarino
- LeftShift, LeftCtrl = Sobe e desce o submarino
- SPACEBAR == Muda o modo da câmera
- MOUSE == Move a direção da câmera no modo câmera livre

## Como Rodar

O zip possui um arquivo main.exe na pasta /bin/Linux, bastando abrir este arquivo para rodar o jogo. 

![image](https://github.com/user-attachments/assets/95b25c7a-d7ce-4b8f-9e29-63b0e8daa296)

Se quiser compilar o jogo em seu pc, é necessário compilar o código usando make run.
