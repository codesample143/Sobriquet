package main

import (
    "fmt"
    "net/http"
    "os"
    "sync"

    "github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
    CheckOrigin: func(r *http.Request) bool {
        return true // allow all connections
    },
}

var (
    clients = make(map[*websocket.Conn]bool)
    mutex   = sync.Mutex{}
)

func handleWebSocket(w http.ResponseWriter, r *http.Request) {
    fmt.Println("New connection request...")
    //upgrades to websocket protocol
    conn, err := upgrader.Upgrade(w, r, nil)
    if err != nil {
        fmt.Println("Upgrade error:", err)
        return
    }
    defer conn.Close()

    mutex.Lock()
    clients[conn] = true
    mutex.Unlock()

    fmt.Println("Client connected:", conn.RemoteAddr())

    for {
        _, msg, err := conn.ReadMessage()
        if err != nil {
            fmt.Println("Read error:", err)
            break
        }

        fmt.Println("Received:", string(msg))

        // Broadcast to all other clients
        mutex.Lock()
        for client := range clients {
            if client != conn {
                err := client.WriteMessage(websocket.TextMessage, msg)
                if err != nil {
                    fmt.Println("Write error:", err)
                    client.Close()
                    delete(clients, client)
                }
            }
        }
        mutex.Unlock()
    }

    // Clean up on disconnect
    mutex.Lock()
    delete(clients, conn)
    mutex.Unlock()
    fmt.Println("Client disconnected:", conn.RemoteAddr())
}

func handleRoot(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintln(w, "WebSocket server is running.")
}

func main() {
    port := os.Getenv("PORT")
    if port == "" {
        port = "10000"
    }

    http.HandleFunc("/", handleRoot)
    http.HandleFunc("/ws", handleWebSocket)

    addr := "0.0.0.0:" + port
    fmt.Println("Listening on", addr)

    if err := http.ListenAndServe(addr, nil); err != nil {
        fmt.Println("Server error:", err)
        os.Exit(1)
    }
}
/*
Alice runs GG(1n) to obtain G, a cyclic group of order q and generator g
Alice chooses a uniform x from G (that is, an integer in Zq)
Alice computes hash hA = gx
Alice sends G, q, g, hA  to Bob  (note that Alice does NOT send x)
Bob  chooses a uniform y from G (that is, an integer in Zq)
Bob computes hash hB = gy   (note that Bob does NOT send y)
Bob sends hB  to A and computes key kB = hAy
Alice computes key kA = hBx   
Observe that kA = kB because (gx)y = (gy)x = (gxy)
*/