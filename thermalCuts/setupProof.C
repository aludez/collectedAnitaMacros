TProof* setupProof(int nworkers = 8)
{
	TProof* proof = TProof::Open("",TString::Format("workers=%d", nworkers));

	proof->Load("macros/loader.C", true);
	proof->Load("macros/cuts.C");

	return proof;
}
