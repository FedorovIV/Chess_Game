function SetFigureOnCell(position, figure, color) {
    elem = document.getElementById(position).firstChild
    if (figure != 'None') {
        elem.className = color + "figure";
        elem.setAttribute("src", "images/" + "B" + figure + ".png");
    } else {
        elem.setAttribute("src", '');
    }

}

//type == 1 - select; type == 0 - no select
function SelectCell(position, type) {
    elem = document.getElementById(position);

    // if (type) {
    //     elem.classList.add("selectB");
    // } else {
    //     elem.classList.remove("selectB");
    // }

    if (type) {
        if (elem.classList.contains("white")) {
            elem.classList.add("selectW");
        } else {
            elem.classList.add("selectB");
        }
    } else {
        if (elem.classList.contains("white")) {
            elem.classList.remove("selectW");
        } else {
            elem.classList.remove("selectB");
        }
    }
}

function getPositionByIJ(i, j) {
    return String.fromCharCode('a'.charCodeAt(0) + j) + (8 - i)
}

function makeBoardfromMassive(arr) {
    let arr2D = new Array(8);

    for (i = 0; i < 8; i++) {
        arr2D[i] = new Array(8);
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++)
            arr2D[i][j] = arr[8 * i + j];
    }

    return arr2D;
}

function fullBoard(arr) {
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {

            var fullName = arr[i][j]

            if (fullName != 'None') {

                var color = fullName[0]
                var name = fullName.substring(1, fullName.length)
                SetFigureOnCell(getPositionByIJ(i, j), name, color)
            }
            else {
                SetFigureOnCell(getPositionByIJ(i, j), 'None')
            }
        }
}

function SelectAllCells(SelCells) {

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            SelectCell(getPositionByIJ(i, j), 0);
        }

    if (SelCells == null) {
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++) {
                SelectCell(getPositionByIJ(i, j), 0);
            }
    } else {
        SelCells.forEach(element => {
            SelectCell(element, 1);
        });
    }
}

//Send to main event Cell ID
async function Cellhandler() {
    window.electronAPI.CellEvent(this.id);
}

async function handleStartButton() {

    const ButtonNext = document.getElementById('ButtonNext');
    const ButtonBack = document.getElementById('ButtonBack');

    ButtonNext.style.visibility = "visible";
    ButtonBack.style.visibility = "visible";

    await window.electronAPI.StartButton();
    // const arr = await window.electronAPI.StartButton();
    // arr2D = makeBoardfromMassive(arr);
    // fullBoard(arr2D);

}

async function handleButtonNext() {
    await window.electronAPI.ButtonNext();
}

async function handleButtonBack() {
    const indicator = document.getElementById('indicator');
    indicator.style.color = "blue";
    await window.electronAPI.ButtonBack();
}


var modalWindow = {
    _block: null,
    _win: null,
    initBlock: function () {
        _block = document.getElementById('blockscreen'); //Получаем наш блокирующий фон по ID

        //Если он не определен, то создадим его
        if (!_block) {
            var parent = document.getElementsByTagName('body')[0]; //Получим первый элемент тега body
            var obj = parent.firstChild; //Для того, чтобы вставить наш блокирующий фон в самое начало тега body
            _block = document.createElement('div'); //Создаем элемент div
            _block.id = 'blockscreen'; //Присваиваем ему наш ID
            parent.insertBefore(_block, obj); //Вставляем в начало
            //_block.onclick = function() { modalWindow.close(); } //Добавим обработчик события по нажатию на блокирующий экран - закрыть модальное окно.
        }
        _block.style.display = 'inline'; //Установим CSS-свойство        
    },
    initWin: function () {
        _win = document.getElementById('modalwindow'); //Получаем наше диалоговое окно по ID
        //Если оно не определено, то также создадим его по аналогии
        if (!_win) {

            const bishop = document.createElement('img');
            const knight = document.createElement('img');
            const queen = document.createElement('img');
            const rook = document.createElement('img');

            bishop.src = './images/Bbishop.png'
            bishop.style.width = '5em'
            knight.src = './images/Bknight.png'
            knight.style.width = '5em'
            queen.src = './images/Bqueen.png'
            queen.style.width = '5em'
            rook.src = './images/Brook.png'
            rook.style.width = '5em'

            bishop.addEventListener('click', async () => {
                await window.electronAPI.ChooseFigure(3);
                modalWindow.close()
            });
            knight.addEventListener('click', async () => {
                await window.electronAPI.ChooseFigure(5);
                modalWindow.close()
            });
            queen.addEventListener('click', async () => {
                await window.electronAPI.ChooseFigure(2);
                modalWindow.close()
            });
            rook.addEventListener('click', async () => {
                await window.electronAPI.ChooseFigure(4);
                modalWindow.close()
            });

            var parent = document.getElementsByTagName('body')[0];
            var obj = parent.firstChild;
            _win = document.createElement('div');
            _win.id = 'modalwindow';
            _win.style.padding = '0 0 5px 0';
            parent.insertBefore(_win, obj);
            _win.appendChild(bishop);
            _win.appendChild(knight);
            _win.appendChild(queen);
            _win.appendChild(rook);


        }

        _win.style.display = 'inline'; //Зададим CSS-свойство

        _win.style.marginLeft = "35%";
        _win.style.top = '50%'; //Позиция по вертикали

    },
    close: function () {
        document.getElementById('blockscreen').style.display = 'none';
        document.getElementById('modalwindow').style.display = 'none';

    },
    show: function () {
        modalWindow.initBlock();
        modalWindow.initWin();
    }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//On-event. Main send to render board in answer on CellID and out this board
window.electronAPI.UpdateBoard((event, ChessBoard, SelCells, MoveMaked) => {
    const indicator = document.getElementById('indicator');
    indicator.style.color = "red";
    SelectAllCells(SelCells);
    arr2D = makeBoardfromMassive(ChessBoard);
    fullBoard(arr2D);
    if (MoveMaked) {
        var audio = new Audio(); // Создаём новый элемент Audio
        audio.src = './audio/move.mp3'; // Указываем путь к звуку "клика"
        audio.autoplay = true; // Автоматически запускаем
    }
});

window.electronAPI.EndGame((event, message) => {
   alert(message);
});

window.electronAPI.ChangePiece((event, message) => {
    modalWindow.show();
 });

 
const WCells = document.querySelectorAll('.white');

WCells.forEach(WCell => {
    WCell.addEventListener('click', Cellhandler);
})

const BCells = document.querySelectorAll('.black');

BCells.forEach(BCell => {
    BCell.addEventListener('click', Cellhandler)
})

const ButtonNext = document.getElementById('ButtonNext');
const ButtonBack = document.getElementById('ButtonBack');

const StartButton = document.getElementById('StartButton')

StartButton.addEventListener("click", handleStartButton, { once: true })

ButtonNext.addEventListener("click", handleButtonNext)

ButtonBack.addEventListener("click", handleButtonBack)

//modalWindow.show(100, "Hello")