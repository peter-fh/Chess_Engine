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

	i++;
	c = fen.charAt(i);
	let turn = c;

	return [board, c];

}


function makePiece(piece, index, board) {

	let img = document.createElement('img');
	img.setAttribute('selected', 'false');
	img.setAttribute('type', piece);
	img.setAttribute('index', index);
	img.setAttribute('moved', 'false');
	img.addEventListener('click', (e) => { onClick(img, e, board) });
	img.src = getPieceImgURL(piece);
	return img;
}


function drawBoard(board, turn) {


	const board_div = document.getElementById('board');

	board_div.setAttribute('turn', turn);


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
		square.id = 'square';
		square.setAttribute('index', i);

		if (lightSquare) {
			square.classList.add('light-square');
		}
		else {
			square.classList.add('dark-square');
		}
		try {
			if (isPiece(board[i])) {
				img = makePiece(board[i], i, board);
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

function isWhite(piece) {
	if (piece.toUpperCase() == piece) {
		return true;
	}
	return false;
}


function take(index, img) {
	console.assert(validate());
	const square = document.querySelector('[index="' + index + '"]');
	console.assert(validate());
	const selected_img = document.querySelector('[selected=true]');
	const old_img = square.querySelector('[selected=false]');
	console.assert(validate());
	square.removeChild(old_img);
	img.setAttribute('selected', 'false');
	console.assert(validate());
}


function isLegalMove(img, game, from, to) {
	const xf = from % 8;
	const yf = Math.floor(from / 8);
	const xt = to % 8;
	const yt = Math.floor(to / 8);
	const dx = xt - xf;
	const dy = yt - yf;
	const piece = img.getAttribute('type');

	function validDiagonalMove() {
		if (game[to] != ' ' && isWhite(game[to]) == isWhite(piece)) {
			return false;
		}
		if (Math.abs(dx) == Math.abs(dy) && dx != 0) {
			const xIterator = dx / Math.abs(dx);
			const yIterator = dy / Math.abs(dy);
			let x = xf + xIterator;
			let y = yf + yIterator;
			while (x != xt && y != yt) {
				if (game[y * 8 + x] != ' ') {
					return false;
				}
				x += xIterator;
				y += yIterator;
			}

			if (game[to] != ' ') {
				take(to, img);
				game[to] = ' ';
				return true;
			}

			return true;

		}
		return false;
	}

	function validHorizontalMove() {
		if (game[to] != ' ' && isWhite(game[to]) == isWhite(piece)) {
			return false;
		}
		if (dy == 0 && dx != 0) {
			const xIterator = dx / Math.abs(dx);
			let x = xf + xIterator;
			while (x != xt) {
				if (game[yf * 8 + x] != ' ') {
					return false;
				}
				x += xIterator;
			}
			return true;
		} else if (dx == 0 && dy != 0) {
			const yIterator = dy / Math.abs(dy);
			let y = yf + yIterator;
			while (y != yt) {
				if (game[y * 8 + xf] != ' ') {
					return false;
				}
				y += yIterator;
			}
			return true;
		}
		return false;
	}
	if (piece == 'p') {

		if (dy == 1 && dx == 0 && game[to] == ' ') {
			return true;
		}
		if (dy == 1 && dx == 1 && game[to] != ' ' && isWhite(game[to])) {
			console.log('taking');
			take(to, img);
			game[to] = ' ';
			return true;
		}
		if (dy == 2 && dx == 0 && yf == 1 && game[to] == ' ' && game[parseInt(to) - 8] == ' ') {
			return true;
		}
	} else if (piece == 'P') {
		if (dy == -1 && dx == 0 && game[to] == ' ') {
			return true;
		} else if (dy == -1 && Math.abs(dx) == 1 && game[to] != ' ' && !isWhite(game[to])) {
			console.log('taking');
			take(to, img);
			game[to] = ' ';
			return true;
		} else if (dy == -2 && dx == 0 && yf == 6 && game[to] == ' ' && game[parseInt(to) + 8] == ' ') {
			return true;
		}
	} else if (piece == 'n' || piece == 'N') {
		if ((Math.abs(dx) == 2 && Math.abs(dy) == 1) || (Math.abs(dx) == 1 && Math.abs(dy) == 2)) {
			if (game[to] == ' ') {
				return true;
			} else if (isWhite(game[to]) != isWhite(piece)) {
				take(to, img);
				game[to] = ' ';
				return true;
			}
		}
	} else if (piece == 'b' || piece == 'B') {
		return validDiagonalMove();
	} else if (piece == 'r' || piece == 'R') {
		return validHorizontalMove();
	} else if (piece == 'q' || piece == 'Q') {
		if (validDiagonalMove() || validHorizontalMove()) {
			return true;
		}
	} else if (piece == 'k' || piece == 'K') {
		if (Math.abs(dx) <= 1 && Math.abs(dy) <= 1) {
			if (game[to] == ' ') {
				return true;
			} else if (isWhite(game[to]) != isWhite(piece)) {
				take(to, img);
				game[to] = ' ';
				return true;
			}
		}
	}


	return false;
}
function validate() {

	var valid = true;
	for (let i = 0; i < 64; i++) {
		const index_divs = document.querySelectorAll('[index="' + i + '"]');
		if (index_divs.length != 2 && index_divs.length != 1) {
			valid = false;
		}
	}
	return valid;
}
function validateBoard(board) {
	var valid = true;
	for (let i = 0; i < 64; i++) {
		const index_divs = document.querySelectorAll('[index="' + i + '"]');
		if (board[i] != ' ') {
			if (index_divs.length != 2) {
				valid = false;
			}
		} else {
			if (index_divs.length != 1) {
				valid = false;
			}

		}
	}

	return valid;


}

function onPickup(img, event, board) {
	img.setAttribute('selected', 'true');

	let squareRect = document.getElementsByClassName('light-square')[0].getBoundingClientRect();
	let squareSize = squareRect.width;
	img.style.position = 'absolute';
	img.style.left = event.pageX - squareSize / 1.75 + 'px';
	img.style.top = event.pageY - squareSize + 'px';
	img.style.width = squareSize + 'px';
	img.style.height = squareSize + 'px';

	img.style.zIndex = 999;
	img.removeEventListener('click', (e) => { onClick(img, e) });
}

function onDrop(img, event, board) {
	console.assert(validateBoard(board));
	console.assert(img.getAttribute('selected') == 'true');
	const origin_square = document.querySelector('[index="' + img.getAttribute('index') + '"]');
	const elements_at_coordinates = document.elementsFromPoint(event.clientX, event.clientY);
	var square_at_coordinates = null;
	for (let i = 0; i < elements_at_coordinates.length; i++) {
		if (elements_at_coordinates[i].id == 'square') {
			square_at_coordinates = elements_at_coordinates[i];
		}
	}

	console.assert(square_at_coordinates != null);
	const to = square_at_coordinates.getAttribute('index');
	const from = img.getAttribute('index');
	if (to == null || !isLegalMove(img, board, from, to)) {
		img.style.zIndex = 1;
		origin_square.appendChild(img);
		img.setAttribute('selected', 'false');
		img.style.position = 'relative';
		img.style.left = 0;
		img.style.top = 0;
	} else {
		img.style.zIndex = 1;
		board[from] = ' ';
		board[to] = img.getAttribute('type');
		//console.log(img);
		img.setAttribute('moved', 'true');
		console.log('square at coordinates', square_at_coordinates);
		square_at_coordinates.appendChild(img);
		var img_at_square = square_at_coordinates.querySelector('[index="' + to + '"]');
		if (img_at_square != null) {
			square_at_coordinates.removeChild(img_at_square);
		}
		console.log(img);
		img.setAttribute('selected', 'false');
		img.setAttribute('index', square_at_coordinates.getAttribute('index'));
		console.assert(validateBoard(board));
		img.style.position = 'relative';
		img.style.left = 0;
		img.style.top = 0;
		board_div = document.getElementById('board');
		if (board_div.getAttribute('turn') == 'w') {
			board_div.setAttribute('turn', 'b');
		} else {
			board_div.setAttribute('turn', 'w');
		}
	}
}


function onClick(img, event, board) {
	if (img.getAttribute('selected') == 'true') {
		onDrop(img, event, board);
	} else {
		board_div = document.getElementById('board');
		if (isWhite(img.getAttribute('type')) && board_div.getAttribute('turn') == 'b') {
			return;
		} else if (!isWhite(img.getAttribute('type')) && board_div.getAttribute('turn') == 'w') {
			return;
		}
		if (document.querySelector('[selected=true]') != null) {
			return;
		}

		onPickup(img, event, board);
	}
}


function onMouseMove(e) {
	try {
		let moved_piece = document.querySelector('[selected=true]');

		if (moved_piece == null) {
			return;
		}

		moved_piece.style.left = e.pageX - moved_piece.width / 1.75 + 'px';
		moved_piece.style.top = e.pageY - moved_piece.width + 'px';
	} catch (error) {
		return;
	}

}


let [board, turn] = fenToBoard('rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1');
console.log(board);
drawBoard(board, turn);
document.addEventListener('mousemove', (e) => { onMouseMove(e); });

