type t;

type data = Js.t {. data : Blob.t};

external create : Navigator.mediaStream => t = "MediaRecorder" [@@bs.new];

external start : t => unit = "start" [@@bs.send];

external stop : t => unit = "stop" [@@bs.send];

external ondataavailable : t => (data => unit) => unit = "ondataavailable" [@@bs.set];

external onstop : t => (data => unit) => unit = "onstop" [@@bs.set];
