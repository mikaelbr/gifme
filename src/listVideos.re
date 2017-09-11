module VideoItem = {
  type state = {
    videoRef: ref (option Dom.element),
    isPlaying: bool
  };
  type actions =
    | Play
    | Stop;
  let setSectionRef theRef {ReasonReact.state: state} => state.videoRef := Js.Null.to_opt theRef;
  let component = ReasonReact.reducerComponent "VideoItem";
  let playVideo video _ =>
    switch !video {
    | None => ()
    | Some r => Video.play r
    };
  let stopVideo video _ =>
    switch !video {
    | None => ()
    | Some r => Video.pause r
    };
  let make ::video _children => {
    ...component,
    initialState: fun () => {videoRef: ref None, isPlaying: false},
    reducer: fun action state =>
      switch action {
      | Play =>
        ReasonReact.UpdateWithSideEffects {...state, isPlaying: true} (playVideo state.videoRef)
      | Stop =>
        ReasonReact.UpdateWithSideEffects {...state, isPlaying: false} (stopVideo state.videoRef)
      },
    didMount: fun self => {
      switch !self.state.videoRef {
      | None => ()
      | Some r =>
        Document.addEventListener r "mouseover" (self.reduce (fun _ => Play));
        Document.addEventListener r "mouseout" (self.reduce (fun _ => Stop))
      };
      ReasonReact.NoUpdate
    },
    render: fun self =>
      <div className="videoItem">
        <video
          ref=(self.handle setSectionRef)
          loop=Js.true_
          className="videoItem__video"
          src=video
        />
      </div>
  };
};

let component = ReasonReact.statelessComponent "VideList";

let make ::videos _children => {
  ...component,
  render: fun _self =>
    <div className="videoList">
      (
        videos |> Array.map (fun video => <VideoItem video key=video />) |> ReasonReact.arrayToElement
      )
    </div>
};
