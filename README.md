<h1 align="center"> Struct Soccer </h1> 
<br> 
<p align="center"> 
    <a href="#"> <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" /> </a> <a href="https://www.raylib.com/"> <img src="https://img.shields.io/badge/Raylib-000000?style=for-the-badge&logoColor=white" /> </a> 
</p>

<h1><strong>⚽ Descrição</strong></h1> <h4> Struct Soccer é um jogo de futebol 3v3 desenvolvido em C com a biblioteca Raylib. A proposta é oferecer uma experiência dinâmica e divertida, onde o jogador controla equipes compostas por estruturas (structs) que simulam atributos reais de um atleta — como velocidade, precisão, energia e posição em campo. Com partidas rápidas e jogabilidade simples, o game combina raciocínio tático e reflexos rápidos, proporcionando diversão e desafio na medida certa. </h4>

<h1><strong>🎥 Vídeo Demonstrativo</strong></h1> <a href="#"> <img src="" alt="Struct Soccer Gameplay"> </a>

<h1><strong>🛠️ Tecnologias Utilizadas</strong></h1>

<div align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" height="40" alt="C logo" />
  <img width="12" />
  <img src="https://www.raylib.com/favicon.ico" height="40" alt="Raylib logo" />
</div>

---

**Estrutura do projeto**

```bash
String-Slayer/
├── assets/             # Imagens e sons   
├── build               # Executáveis
├── include/            # Headers do projeto
├── src/                # Códigos-fonte dos minigames
├── Makefile            # Script de build
└── README.md           # README.md
``` 

<h1><strong>🚀 Instalação e Execução</strong></h1>

### ✅ Pré-requisitos

- Distribuição Linux (Ubuntu/Debian recomendada)
- Compilador `gcc`
- `make` e `cmake`
- Biblioteca gráfica `raylib`

---
### 🔧 Passo a passo

1. Atualize os pacotes do sistema
Execute os seguintes comandos para garantir que seu sistema esteja atualizados
```
sudo apt update && sudo apt upgrade -y
```

2. **Instale depêndencias básicas de compilação**
```bash
sudo apt install -y build-essential make cmake git
````

3. **Instale as bibliotecas gráficas necessárias**
```bash
sudo apt install -y libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxcursor-dev libxrandr-dev libxi-dev libxinerama-dev xorg-dev xserver-xorg-dev libopenal-dev libasound2-dev
```

4. **Instale a raylib**
💡 Se preferir usar a versão mais recente da raylib, siga o método manual abaixo. 
```bash
cd ~
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -DPLATFORM=Desktop ..
make
sudo make install
```
✅ Caso já tenha a raylib instalada, você pode pular este passo

5. **Clone o Repositório**
   
```bash
git clone https://github.com/David-git123/StructSoccer.git
cd StructSoccer
```

6. **Execução do Jogo**
Para compilar e rodar o jogo
```bash
make run
```
Apenas compilar (sem executar)
```bash
make
```
Limpar arquivos objeto:
```bash
make clean
```

<h1> 🎉 Obrigado por instalar o Struct Soccer! </h1>
Esperamos que você se divirta jogando tanto quanto nós nos divertimos criando! ❤️ 
Bom jogo!

<br>
<h1><strong>👥 Equipe</strong></h1>

<p align="center">
<table>
  <tr>
    <td align="center">
      <a href="https://github.com/caioguimaraesr">
        <img src="https://avatars.githubusercontent.com/caioguimaraesr" width="100" style="border-radius: 50%; margin: 10px;" />
        <br><strong>🎮 Caio G. Rocha</strong>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/rafaferraz05">
        <img src="https://avatars.githubusercontent.com/rafaferraz05" width="100" style="border-radius: 50%; margin: 10px;" />
        <br><strong>🎮 Rafael Ferraz</strong>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/David-git123">
        <img src="https://avatars.githubusercontent.com/David-git123" width="100" style="border-radius: 50%; margin: 10px;" />
        <br><strong>🎮 David Magalhães</strong>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Cavalcanti-Felipe">
        <img src="https://avatars.githubusercontent.com/Cavalcanti-Felipe" width="100" style="border-radius: 50%; margin: 10px;" />
        <br><strong>🎮 Felipe Cavalcanti</strong>
      </a>
    </td>
  </tr>
</table>
</p>
