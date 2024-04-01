// Oops! Something went wrong! :(
function isNumber(c) { return !isNaN(parseInt(c)); }


function isPiece(c) { return c != ' '; }


function getPieceImgURL(piece) {
	const pieceMap = new Map();
	pieceMap.set('K', 'kw');
	pieceMap.set('Q', 'qw');
	pieceMap.set('R', 'rw');
	pieceMap.set('B', 'bw');
	pieceMap.set('N', 'nw');
	pieceMap.set('P', 'pw');
	pieceMap.set('k', 'kb');
	pieceMap.set('q', 'qb');
	pieceMap.set('r', 'rb');
	pieceMap.set('b', 'bb');
	pieceMap.set('n', 'nb');
	pieceMap.set('p', 'pb');
	url = 'res/' + pieceMap.get(piece) + '.png';
	return url;

}


function fenToBoard(fen) {
	let index = 63;
	let board = [];
	let c = fen.charAt(0);
	let i = 0;
	while (c != ' ') {
		if (isNumber(c)) {
			for (let i = 0; i < parseInt(c); i++) {
				board[63 - index] = ' ';
				index--;
			}
		} else if (c === '/') {
			board[63 - index] = ' ';
		} else {
			board[63 - index] = c;
			index--;
		}
		i++;
		c = fen.charAt(i);
	}

	return board;

}


function makePiece(piece) {

	let img = document.createElement('img');
	img.setAttribute('selected', 'false');
	img.addEventListener('click', (e) => { onClick(img, e) });
	img.src = getPieceImgURL(piece);
	return img;
}


function displayBoard(board) {


	const board_div = document.getElementById('board');


	for (let i = 0; i < 64; i++) {

		// stupid ass modulus logic for board
		// modulus by 2 doesn't stay consistent because of alternating pattern
		let lightSquare = false;

		if (Math.floor(i / 8) % 2 === 0 && (i % 2) === 0) {
			lightSquare = true;
		} else if (Math.floor(i / 8) % 2 === 1 && (i % 2) === 1) {
			lightSquare = true;
		}

		const square = document.createElement('div');

		if (lightSquare) {
			square.classList.add('light-square');
		}
		else {
			square.classList.add('dark-square');
		}
		try {
			if (isPiece(board[i])) {
				img = makePiece(board[i]);
				square.appendChild(img);
			}
		} catch (error) {
			console.log(i);
		}
		if (i === 0) {
			square.style.borderRadius = "10px 0px 0px 0px";
		} else if (i == 7) {
			square.style.borderRadius = "0px 10px 0px 0px";
		} else if (i == 56) {
			square.style.borderRadius = "0px 0px 0px 10px";
		} else if (i == 63) {
			square.style.borderRadius = "0px 0px 10px 0px";
		}


		board_div.appendChild(square);

		// empty div acts as newline
		if ((i + 1) % 8 === 0) {
			const empty = document.createElement('div');
			board_div.appendChild(empty);
		}
	}
}


function onClick(img, event) {
	var selected = img.getAttribute('selected');
	if (selected == 'true') {
		const square_at_coordinates = document.elementsFromPoint(event.clientX, event.clientY)[1];
		const squareRect = square_at_coordinates.getBoundingClientRect();
		console.log(square_at_coordinates);
		square_at_coordinates.appendChild(img);
		img.setAttribute('selected', 'false');
		img.style.position = 'relative';
		img.style.left = 0;
		img.style.top = 0;

		return;
	}
	img.setAttribute('selected', 'true');

	let squareRect = document.getElementsByClassName('light-square')[0].getBoundingClientRect();
	let squareSize = squareRect.width;
	console.log(squareSize);
	img.style.position = 'absolute';
	img.style.left = event.pageX - squareSize / 1.75 + 'px';
	img.style.top = event.pageY - squareSize + 'px';
	img.style.width = squareSize + 'px';
	img.style.height = squareSize + 'px';

	img.style.zIndex = 999;
	img.removeEventListener('click', (e) => { onClick(img, e) });
}


function onMouseMove(e) {
	try {
		let moved_piece = document.querySelector('[selected=true]');

		if (moved_piece == null) {
			return;
		}
		
		moved_piece.style.position = 'absolute';
		moved_piece.style.left = e.pageX - moved_piece.width / 1.75 + 'px';
		moved_piece.style.top = e.pageY - moved_piece.width + 'px';
	} catch (error) {
		return;
	}

}


let board = fenToBoard('rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ');
displayBoard(board);
document.addEventListener('mousemove', (e) => { onMouseMove(e); });

