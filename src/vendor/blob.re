type t = Js.t {. size : int, type_ : string};

type inputType =
  | Array (Js.Array.t string)
  | Blob t;

type data = Js.t {. data : string};

external blob : inputType => t = "Blob" [@@bs.new];
