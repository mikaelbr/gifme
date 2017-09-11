external get_time : unit => int = "Date.now" [@@bs.val];

let rootDir = Node_process.cwd ();

let storeDir = Node_path.join [|rootDir, "stored-videos"|];

let filename () =>
  Node_path.join [|rootDir, "stored-videos", string_of_int (get_time ())|] ^ ".webm";
