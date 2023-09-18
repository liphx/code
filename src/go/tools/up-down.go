package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"strconv"
)

func usage() {
	fmt.Println(`Usage:
	up <pathname>
	down <ip> <port>`)
}

func handleConnection(pathname string, conn net.Conn) {
	defer conn.Close()
	fmt.Println("Remote addr:", conn.RemoteAddr())
	file, err := os.Open(pathname)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()
	var buffer [512]byte
	for {
		n, err := file.Read(buffer[:])
		if err == io.EOF {
			break
		}
		if err != nil {
			fmt.Println(err)
			return
		}
		for n > 0 {
			n2, err := conn.Write(buffer[:n])
			if err != nil {
				fmt.Println(err)
				return
			}
			n -= n2
		}
	}
}

func up(pathname string) {
	// fmt.Println("up", pathname)
	ln, err := net.Listen("tcp", "")
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("Listen addr:", ln.Addr())
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println(err)
			continue
		}
		go handleConnection(pathname, conn)
	}
}

func down(ip string, port int) {
	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%d", ip, port))
	if err != nil {
		fmt.Println(err)
		return
	}
	defer conn.Close()
	fmt.Println("Local addr:", conn.LocalAddr())
	file, err := os.Create("received")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()
	var buffer [512]byte
	for {
		n, err := conn.Read(buffer[:])
		if err == io.EOF {
			fmt.Println("File is saved as './received'")
			return
		}
		if err != nil {
			fmt.Println(err)
			return
		}
		n2, err := file.Write(buffer[:n])
		if err != nil {
			fmt.Println(err)
			return
		}
		if n2 != n {
			fmt.Println("Write file fail")
			return
		}
	}
}

func main() {
	if len(os.Args) < 2 {
		usage()
		return
	}
	if os.Args[1] == "up" {
		if len(os.Args) != 3 {
			usage()
			return
		}
		up(os.Args[2])
	} else if os.Args[1] == "down" {
		if len(os.Args) != 4 {
			usage()
			return
		}
		port, err := strconv.Atoi(os.Args[3])
		if err != nil {
			fmt.Println(err)
			return
		}
		down(os.Args[2], port)
	} else {
		usage()
	}
}
