package main

import (
    "fmt"
    "bytes"
    "encoding/json"
    "net/http"
    "io"
)


const engineEndpoint string = "http://127.0.0.1:6969/get_engine_move"


func callEngine(fen string) (*http.Response, error) {
    fenMap := map[string]string{"Board": fen}

    jsonMap, err := json.Marshal(fenMap)
    if err != nil {
	panic("Test suite failed to convert fen to json")
    }
    
    jsonReader := bytes.NewReader(jsonMap)

    response, err := http.Post(engineEndpoint, "application/json", jsonReader)
   
    return response, err



}


func TestHTTPCall(){
    fens := [...]string{
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",
	"r1bqkbnr/pppppppp/n7/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	"rnbqkb1r/pppppppp/8/4n3/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
	"rn1qkb1r/ppp1pppp/8/3p4/3Pn3/5N2/PPPN1PPP/R1BQKB1R b KQkq - 0 1",
	"rnbqkb1r/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
	"rnbqkb1r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
	"r1bqkbnr/pppppppp/2n5/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    }

    for _, fen := range fens {
	res, err := callEngine(fen)
	if err != nil {
	    fmt.Println("error in calling fen", fen)
	    fmt.Println(err)
	}


	printBody(res)

    }
}


func printBody(res *http.Response) error{
    body, err := io.ReadAll(res.Body)
    
    if err != nil {
	return err
    }
    
    fmt.Println(string(body))
    return nil
}

func main(){
    TestHTTPCall()
}
