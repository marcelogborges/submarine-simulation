# Submarine Simulation
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
  - **Câmeras**: A implementação do sistema de câmeras oferece dois modos: LookAt e Free Camera, alternando entre eles com base na interação do usuário. Ambos os modos utilizam matrizes de transformação para atualizar a posição e a orientação da câmera em tempo real, garantindo uma experiência visual fluida e consistente.

    - LookAt: a câmera acompanha o submarino automaticamente. Sua posição é calculada a uma distância fixa atrás e acima do submarino, e o vetor de direção (View) é obtido a partir da diferença entre a posição do submarino e da câmera. O vetor Up permanece constante, garantindo que a câmera mantenha uma orientação estável.

    - Câmera Livre: o usuário controla a câmera com o teclado e o mouse. A rotação é calculada com base no movimento do cursor, limitando o ângulo vertical para evitar ultrapassar o horizonte. A posição é atualizada conforme as teclas pressionadas, permitindo movimento fluido em todas as direções, com velocidade ajustada pelo delta time.
  
  - **Movimentação**: A movimentação do submarino foi desenvolvida com base em transformações geométricas e conceitos básicos de física. O sistema combina rotação e translação por meio de matrizes, onde a matriz final resulta da multiplicação da translação pela matriz de rotação no eixo Y. A direção do submarino é atualizada por um vetor normalizado que reflete a rotação atual, enquanto a velocidade horizontal é obtida multiplicando a direção pelo módulo da velocidade, com aceleração e desaceleração suaves para simular inércia. O movimento vertical é independente, permitindo deslocamento nos dois eixos ao somar as velocidades horizontal e vertical, garantindo um movimento tridimensional fluido. A rotação é controlada por ângulos normalizados entre -π e π para evitar overflow, e todas as transformações utilizam delta time, assegurando fluidez do movimento, independentemente da taxa de quadros.
  
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
- A, D == Rotaciona o submarino
- W, S == Acelera e freia o submarino
- LeftShift, LeftCtrl = Sobe e desce o submarino
- SPACEBAR == Muda o modo da câmera
- MOUSE == Move a direção da câmera no modo câmera livre

## Como Rodar
O zip possui um arquivo main.exe na pasta /bin/Linux, bastando abrir este arquivo para rodar o jogo. 
Se quiser compilar o jogo em seu pc, é necessário compilar o código usando make run.



![image](https://github.com/user-attachments/assets/8ecc2dc6-aff9-4055-971b-6dad0d30010f)
![image](https://github.com/user-attachments/assets/749b7050-8371-4bcb-beff-1f9bdd5e0743)


