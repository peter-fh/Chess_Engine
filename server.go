package main 


import (
    "fmt"
    "net/http"
    "encoding/json"
    "io"
    "os/exec"
    "errors"
)


type MoveRequest struct {

    Board string `json:"Board"`
}


func exec_engine(fen string) (string, error) {
    engineCmd := exec.Command("engine/build/Engine", fen)

    new_board, err := engineCmd.Output()
    if err != nil {
	return "", errors.New("error in engine exec")
    }
    return string(new_board), nil
}

func get_engine_move(w http.ResponseWriter, req *http.Request){
    fmt.Println("get_engine_move called")
    if req.Method != http.MethodPost {
	http.Error(w, "POST is required for engine move", http.StatusMethodNotAllowed)
	return
    }

    body, err := io.ReadAll(req.Body)
    if err != nil {
	http.Error(w, "Unable to read body", http.StatusBadRequest)
	return
    }
    defer req.Body.Close()

    var moveRequest MoveRequest
    err = json.Unmarshal(body, &moveRequest)
    if err != nil {
	http.Error(w, "Invalid JSON sent", http.StatusBadRequest)
	return
    }

    new_board, err := exec_engine(moveRequest.Board)
    if err != nil {
	http.Error(w, "Invalid board state", http.StatusBadRequest)
	return
    }
    fmt.Println("recieved", moveRequest.Board)
    fmt.Println("sent", new_board)
    fmt.Fprint(w, new_board)

}


func main() {
    http.HandleFunc("/get_engine_move", get_engine_move)
    fmt.Println("Starting server on port :6969")
    err := http.ListenAndServe("127.0.0.1:6969", nil)
    if err != nil {
	fmt.Printf("Unable to start server: %v\n", err)
    }
}
