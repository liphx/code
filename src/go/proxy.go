package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

func fetch(url string) []byte {
	resp, err := http.Get(url)
	if err != nil {
		log.Print(err)
		return nil
	}
	b, err := ioutil.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		log.Print(err)
		return nil
	}
	return b
}

func main() {
	http.HandleFunc("/", proxy)
	log.Fatal(http.ListenAndServe("localhost:8000", nil))
}

func proxy(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "%s %s %s\n", r.Method, r.URL, r.Proto)
	resp := fetch(r.URL.String())
	if resp != nil {
		fmt.Fprintf(w, string(resp))
	}
}
