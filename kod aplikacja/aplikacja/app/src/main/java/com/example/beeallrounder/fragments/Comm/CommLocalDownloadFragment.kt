package com.example.beeallrounder.fragments.Comm

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.beeallrounder.LocalComm.TcpClient
import com.example.beeallrounder.R
import com.example.beeallrounder.databases.oldDB.model.Beehive_snapshot
import com.example.beeallrounder.databases.oldDB.viewmodel.UserViewModelOld
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.util.*

class CommLocalDownloadFragment : Fragment() {

    private lateinit var mUserViewModel: UserViewModelOld

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        mUserViewModel = ViewModelProvider(this).get(UserViewModelOld::class.java)
        return inflater.inflate(R.layout.fragment_comm_local_download, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val btnCommLocalDownloadDownloadAll = view.findViewById<Button>(R.id.btnCommLocalDownloadDownloadAll)
        btnCommLocalDownloadDownloadAll.setOnClickListener {
            downloadAll()
        }


    }

    private fun downloadAll() {
        CoroutineScope(Dispatchers.Default).launch {
            Handler(Looper.getMainLooper()).post {
                Toast.makeText(requireContext(),R.string.Starting,Toast.LENGTH_SHORT).show()
            }
            val wynik = TcpClient.DownloadAll()
            if(wynik[0] as Boolean) {
                Handler(Looper.getMainLooper()).post {
                    if (wynik[1] == null) {
                        Toast.makeText(requireContext(),R.string.ToastSuccessfulyReceivedMsgUnidentified,Toast.LENGTH_LONG).show()
                    }
                    else {
                        Toast.makeText(requireContext(), getString(R.string.ToastSuccessfulyReceivedMsgIdentified) + (wynik[1] as String),Toast.LENGTH_LONG).show()
                        addToDB(wynik[1] as String)
                        try {
                            val file = context?.getFileStreamPath("dane.txt")
                            if( file == null || !file.exists() ) {
                                // wiem że mogę w obu przypadkach dać append ale chcę mieć możliwość czyszczenia
                                //Log.d("Zapis","tworzę nowy plik")
                                context?.openFileOutput("dane.txt", Context.MODE_PRIVATE).use {
                                    it?.write(   (   (wynik[1] as String)+"\n"    ).toByteArray())
                                }
                            }
                            else {
                                //Log.d("Zapis","dopisuje do pliku")
                                context?.openFileOutput("dane.txt", Context.MODE_APPEND + Context.MODE_PRIVATE).use {
                                    it?.write((wynik[1] as String).toByteArray())
                                }
                            }
                        } catch (e: Exception) {
                            //Log.e("Error plikow", "Cos nie pyklo z plikami")
                        }
                    }
                }
            }
            else {
                Handler(Looper.getMainLooper()).post {
                    Toast.makeText(requireContext(),R.string.ToastFailedReceiveMsg,Toast.LENGTH_LONG).show()
                }
            }

        }

    }

    private fun addToDB(czas: String) {
        val now = Calendar.getInstance().getTime();
        val snapshot = Beehive_snapshot(0,now.toString(),1,czas)
        mUserViewModel.addBeehive(snapshot)
    }

}