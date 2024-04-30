echo "## Compiling engine ##"

emcc engine.cpp -o engine.js \
    Engine/Search.cpp \
    Engine/Board.cpp \
    Engine/Bithack.cpp \
    Engine/Move.cpp \
    Engine/Moves.cpp \
    Engine/Fen.cpp \
    -s EXPORTED_FUNCTIONS="['_engineMove', '_freeStr']" \
    -s EXPORTED_RUNTIME_METHODS="['stringToNewUTF8']" \
    -s ALLOW_MEMORY_GROWTH \
    -s MAXIMUM_MEMORY=4GB

echo '## Done ##'
