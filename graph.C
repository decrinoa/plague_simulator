void graph () {
   
  TCanvas *plague = new TCanvas("plague", "Epidemiologic Graph");

  TGraph * I = new TGraph("data.txt", "%lg %*lg %lg %*lg %*lg");
  
  TGraph * S = new TGraph("data.txt", "%lg %lg %*lg %*lg %*lg");

  TGraph * R = new TGraph("data.txt", "%lg %*lg %*lg %lg %*lg");

  TGraph * Q = new TGraph("data.txt", "%lg %*lg %*lg %*lg %lg");


     TMultiGraph *mg = new TMultiGraph();
   mg->SetTitle("Epidemiologic Graph");
  
  I->SetLineColor(2);
  I->SetLineWidth(4);
  S->SetLineColor(kGreen);
  S->SetLineWidth(4);
  R->SetLineColor(kBlue);
  R->SetLineWidth(4);
  Q->SetLineColor(1);
  Q->SetLineWidth(4);
  
  plague->SetFillColor(0);
  
  mg->Add(I);
  mg->Add(S);
  mg->Add(R);
  mg->Add(Q);
  mg->Draw("AC");

  
  TLegend *leg=new TLegend(.1,.7,.3,.9,"Plague-Simulator");
  leg->SetFillColor(0);
  leg->AddEntry(I,"Infected");
  leg->AddEntry(S,"Susceptible");
  leg->AddEntry(R,"Recovered");
  leg->AddEntry(Q,"People in quarantine");
  leg->Draw("Same");

 }
